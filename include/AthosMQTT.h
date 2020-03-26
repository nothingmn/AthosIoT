#ifdef ATH_MQTT
#include <PubSubClient.h>
#include <esp8266httpupdate.h>

WiFiClient mqtt_wclient; //Declares a WifiClient Object using ESP8266WiFi
PubSubClient mqtt_client(mqtt_wclient);
String _mqtt_deviceId;
StorageValues _mqtt_config;

bool ConnectToMqtt()
{

  if (!mqtt_client.connected())
  {
    //client object makes connection to server
    String clientId = "ATH" + _mqtt_deviceId;

    //Authenticating the client object
    while (!mqtt_client.connected())
    {
      int result = 0;
      if (_mqtt_config.mqttUsername != NULL && _mqtt_config.mqttPassword != NULL && _mqtt_config.mqttUsername != "" && _mqtt_config.mqttPassword != "")
      {
        Log.trace("MQTT Using Password authentication");
        result = mqtt_client.connect(clientId.c_str(), _mqtt_config.mqttUsername.c_str(), _mqtt_config.mqttPassword.c_str());
      }
      else
      {
        Log.trace("MQTT NOT using authentication");
        result = mqtt_client.connect(clientId.c_str());
      }
      if (result)
      {
        Log.trace("MQTT Connected");
      }
      else
      {
        Log.trace("failed, mqtt state:\"%d\", try again in 5 seconds",mqtt_client.state());
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
    return true;
  }
  return false;
}
String GetValueOrDefault(StaticJsonDocument<256> doc, String group, String name, String defaultValue)
{
  String v = doc[group][name].as<String>();
  if (v == NULL || v == "" || v == "null")
  {
    return defaultValue;
  }
  return v;
}


void MQTT_PongResponse(int senderTS)
{
  Log.trace("MQTT_PongResponse");
  int ts = NTP_getEpochTime();
  int diff = ts - senderTS;
  StaticJsonDocument<200> doc;
  doc["command"] = "pong";
  doc["senderTS"] = senderTS;
  doc["ts"] = ts;
  doc["diff"] = diff;
  doc["deviceid"] = _mqtt_deviceId;
  String json;
  serializeJson(doc, json);
  Log.trace(json.c_str());
  Log.trace(_mqtt_config.mqttPingTopic.c_str());
  mqtt_client.publish(_mqtt_config.mqttPingTopic.c_str(), json.c_str());
  MQTTTransmitLed();
}

void callback(char *topic, byte *payload, unsigned int length)
{

  payload[length] = '\0';
  String json = String((char *)payload);
  String strTopic = String(topic);

  Log.trace("mqtt payload received");
  Log.trace(json.c_str());
  Log.trace(strTopic.c_str());

  bool handled = false;

#ifdef ATH_RELAY
  handled = Relay_MQTT_Received(topic, payload, length);
#endif

  if (!handled)
  {
    Log.trace("mqtt payload not handled by the Relay node, inspecting for system commands");
    StaticJsonDocument<256> readDoc;

    deserializeJson(readDoc, json);
    String command = readDoc["command"].as<String>();
    if (command == "reset")
    {
      ESP.reset();
    }
    else if (command == "restart")
    {
      ESP.restart();
    }
    else if (command == "upgrade") {
        String host = readDoc["host"].as<String>();
        int port = readDoc["port"].as<int>();
        String path = readDoc["path"].as<String>();

        if(host != NULL && host != "" && host != "null" && !isnan(port) && path != NULL && path != "" && path != "null") {
            Log.notice("UPDATE RECEIVED FROM SERVER [%s]:[%i][%s]", host.c_str(), port, path.c_str());
            ESPhttpUpdate.update(host, port, path);
        }
    }
    else if (command == "wipe")
    {
      wipeEEPROM();
      ESP.reset();
      Log.trace("EEPROM KILLED!");
    }
    else if(command == "ping") {
      String _ts = readDoc["ts"].as<String>();
      MQTT_PongResponse(atoi(_ts.c_str()));
    }
    else if (command == "reconfigure")
    {
      StorageValues config = readEEPROMData();

      _mqtt_config.ssid = GetValueOrDefault(readDoc, "wifi", "ssid", config.ssid);
      _mqtt_config.password = GetValueOrDefault(readDoc, "wifi", "password", config.password);

      _mqtt_config.mqttRelayTopic = GetValueOrDefault(readDoc, "mqtt", "relay", config.mqttRelayTopic);
      _mqtt_config.mqttCapsTopic = GetValueOrDefault(readDoc, "mqtt", "caps", config.mqttCapsTopic);
      _mqtt_config.mqttPingTopic = GetValueOrDefault(readDoc, "mqtt", "ping", config.mqttPingTopic);
      
      _mqtt_config.mqttSensorTopic = GetValueOrDefault(readDoc, "mqtt", "sensor", config.mqttSensorTopic);
      _mqtt_config.mqttServer = GetValueOrDefault(readDoc, "mqtt", "server", config.mqttServer);
      _mqtt_config.mqttUsername = GetValueOrDefault(readDoc, "mqtt", "username", config.mqttUsername);
      _mqtt_config.mqttPassword = GetValueOrDefault(readDoc, "mqtt", "password", config.mqttPassword);
      _mqtt_config.mqttPort = GetValueOrDefault(readDoc, "mqtt", "port", config.mqttPort);

      writeEEPROMData(_mqtt_config);
      ESP.restart();
    }
    readDoc.clear();
  }
}

void ConnectAndSubscribe()
{
  if (ConnectToMqtt())
  {
    mqtt_client.subscribe(_mqtt_config.mqttRelayTopic.c_str());
    Log.trace("Setup MQTT Subscriber complete:%s", _mqtt_config.mqttRelayTopic.c_str());
  }
}
PubSubClient MQTT_Setup(String deviceId, StorageValues rootConfig)
{
  _mqtt_deviceId = deviceId;
  _mqtt_config = rootConfig;
  Log.trace("Connecting to MQTT server: %s %s", _mqtt_config.mqttServer.c_str(), _mqtt_config.mqttPort.c_str());
  mqtt_client.setServer(_mqtt_config.mqttServer.c_str(), _mqtt_config.mqttPort.toInt());
  mqtt_client.setCallback(callback);

  ConnectAndSubscribe();

  return mqtt_client;
}

void MQTT_Loop()
{
  ConnectAndSubscribe();
  mqtt_client.loop();
}

#endif
