//AthosMQTT.cpp
#include "AthosEEPROM.h"
#include "AthosMQTT.h"
#include "AthosHelpers.h"
#include "AthosNTP.h"
#include <PubSubClient.h>
#include "Arduino.h"
#include <ArduinoLog.h>
#include <ESP8266httpUpdate.h>

WiFiClient mqtt_wclient; //Declares a WifiClient Object using ESP8266WiFi
PubSubClient mqtt_client(mqtt_wclient);
String _mqtt_deviceId;
StorageValues _mqtt_config;
AthosHelpers _mqtt_helpers;
AthosNTP _mqtt_ntp;
AthosEEPROM _mqtt_eeprom;
#ifdef ATH_RELAY
    AthosRelay _mqtt_relay;
#endif

bool AthosMQTT::ConnectToMqtt(void)
{

  if (!mqtt_client.connected())
  {
    Log.trace("MQTT not connected, we are going to connect to it now..");
    //client object makes connection to server
    String clientId = "ATH" + _mqtt_deviceId;
    const char* _clientId = clientId.c_str();
    //Authenticating the client object
    while (!mqtt_client.connected())
    {
      int result = 0;
      if (_mqtt_config.mqttUsername != NULL && _mqtt_config.mqttPassword != NULL && _mqtt_config.mqttUsername != "" && _mqtt_config.mqttPassword != "")
      {
        Log.trace("MQTT Using Password authentication ClientID:%s", _clientId);
        result = mqtt_client.connect(_clientId, _mqtt_config.mqttUsername.c_str(), _mqtt_config.mqttPassword.c_str());
      }
      else
      {
        Log.trace("MQTT NOT using authentication ClientID:%s", _clientId);
        result = mqtt_client.connect(_clientId);
        Log.trace("XXXX MQTT NOT using authentication ClientID:%s", _clientId);

      }
      if (result)
      {
        Log.trace("MQTT Connected");
      }
      else
      {
        Log.trace("failed, mqtt state:\"%i\", try again in 5 seconds",mqtt_client.state());
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
    return true;
  }
  return false;
}


String AthosMQTT::GetValueOrDefault(StaticJsonDocument<256> doc, String group, String name, String defaultValue)
{
  String v = doc[group][name].as<String>();
  if (v == NULL || v == "" || v == "null")
  {
    return defaultValue;
  }
  return v;
}


void AthosMQTT::MQTT_PongResponse(int senderTS)
{
  Log.trace("MQTT_PongResponse");
  int ts = _mqtt_ntp.NTP_getEpochTime();
  int diff = abs(ts - senderTS);

  String csv = String("PONG," + _mqtt_helpers.getVersion() + "," + ts + "," + senderTS + "," + diff + ","+ _mqtt_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _mqtt_config.mqttPingTopic.c_str();
  Log.trace("Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!mqtt_client.publish(topic, payload))
  {
    Log.trace("PONG Data to MQTT Failed. Packet > 128?");
  }
  _mqtt_helpers.MQTTTransmitLed();
}

void AthosMQTT::MQTT_Callback(char *topic, uint8_t *payload, unsigned int length)
{
  Log.trace("MQTT_Callback");

  payload[length] = '\0';
  String json = String((char *)payload);
  String strTopic = String(topic);

  Log.trace("mqtt payload received");
  Log.trace(json.c_str());
  Log.trace(strTopic.c_str());

  bool handled = false;

// #ifdef ATH_RELAY
//   handled = _mqtt_relay.Relay_MQTT_Received(strTopic, json);
// #endif

  if (!handled)
  {
    Log.trace("mqtt payload not handled by the Relay node, inspecting for system commands");
    StaticJsonDocument<256> readDoc;

    deserializeJson(readDoc, json);
    String command = readDoc["command"].as<String>();
    if (command == "reset")
    {
      Log.trace("Reset command received");
      ESP.reset();
      delay(1000);
    }
    else if (command == "restart")
    {
      Log.trace("Restart command received");
      ESP.restart();
      delay(1000);
    }
    else if (command == "upgrade") {
      Log.trace("Upgrade command received");
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
      Log.trace("Wipe command received");
      _mqtt_eeprom.wipeEEPROM();
      ESP.reset();
      Log.trace("EEPROM KILLED!");
    }
    else if(command == "ping") {
      Log.trace("Ping command received");
      String _ts = readDoc["ts"].as<String>();
      MQTT_PongResponse(atoi(_ts.c_str()));
    }
    else if (command == "reconfigure")
    {
      Log.trace("Reconfigure command received");
      StorageValues config = _mqtt_eeprom.readEEPROMData();

      _mqtt_config.ssid = GetValueOrDefault(readDoc, "wifi", "ssid", config.ssid);
      _mqtt_config.password = GetValueOrDefault(readDoc, "wifi", "password", config.password);

      _mqtt_config.mqttRelayTopic = GetValueOrDefault(readDoc, "mqtt", "relay", config.mqttRelayTopic);
      _mqtt_config.mqttPingTopic = GetValueOrDefault(readDoc, "mqtt", "ping", config.mqttPingTopic);
      _mqtt_config.mqttMotionTopic = GetValueOrDefault(readDoc, "mqtt", "motion", config.mqttMotionTopic);
      
      _mqtt_config.mqttSensorTopic = GetValueOrDefault(readDoc, "mqtt", "sensor", config.mqttSensorTopic);
      _mqtt_config.mqttServer = GetValueOrDefault(readDoc, "mqtt", "server", config.mqttServer);
      _mqtt_config.mqttUsername = GetValueOrDefault(readDoc, "mqtt", "username", config.mqttUsername);
      _mqtt_config.mqttPassword = GetValueOrDefault(readDoc, "mqtt", "password", config.mqttPassword);
      _mqtt_config.mqttPort = GetValueOrDefault(readDoc, "mqtt", "port", config.mqttPort);

      _mqtt_eeprom.writeEEPROMData(_mqtt_config);
      ESP.restart();
    }
    readDoc.clear();
  }
}

void AthosMQTT::ConnectAndSubscribe(void)
{
  if (ConnectToMqtt())
  {
    mqtt_client.subscribe(_mqtt_config.mqttRelayTopic.c_str());
    Log.trace("Setup MQTT Subscriber complete:%s", _mqtt_config.mqttRelayTopic.c_str());
  }
}
PubSubClient AthosMQTT::MQTT_Setup(String deviceId, StorageValues rootConfig)
{
  _mqtt_deviceId = deviceId;
  _mqtt_config = rootConfig;
  Log.trace("Connecting to MQTT server: %s %s", _mqtt_config.mqttServer.c_str(), _mqtt_config.mqttPort.c_str());
  mqtt_client.setServer(_mqtt_config.mqttServer.c_str(), _mqtt_config.mqttPort.toInt());

  // using std::placeholders::_1;
  // using std::placeholders::_2;
  // using std::placeholders::_3;
  // mqtt_client.setCallback(std::bind( &AthosMQTT::MQTT_Callback, this, _1,_2,_3));

  //  mqtt_client.setCallback(this->MQTT_Callback);

  ConnectAndSubscribe();

  return mqtt_client;
}

void AthosMQTT::MQTT_Loop(void)
{
  ConnectAndSubscribe();
  mqtt_client.loop();
}
#ifdef ATH_RELAY
  void AthosMQTT::InitRelay(AthosRelay relay) {
    _mqtt_relay = relay;
  }
#endif

/*
  Constructor
*/
AthosMQTT::AthosMQTT()
{
}

AthosMQTT::AthosMQTT(AthosHelpers helpers, AthosNTP ntp, AthosEEPROM eeprom)
{
  _mqtt_helpers = helpers;
  _mqtt_ntp = ntp;  
  _mqtt_eeprom = eeprom;  
}



