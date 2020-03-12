#ifdef ATH_MQTT
#include <PubSubClient.h>

WiFiClient mqtt_wclient; //Declares a WifiClient Object using ESP8266WiFi
PubSubClient mqtt_client(mqtt_wclient);
String _mqtt_deviceId;
StorageValues _mqtt_config;

bool ConnectToMqtt()
{

  if (!mqtt_client.connected())
  {
    //client object makes connection to server
    String clientId = "ESP" + _mqtt_deviceId;
    Serial.print("MQTT clientid=");
    Serial.println(clientId);
    //Authenticating the client object
    while (!mqtt_client.connected())
    {
      int result = 0;
      if(_mqtt_config.mqttUsername != NULL &&_mqtt_config.mqttPassword != NULL && _mqtt_config.mqttUsername != "" && _mqtt_config.mqttPassword != "") {
        Serial.println("MQTT Using Password authentiation");
        result = mqtt_client.connect(clientId.c_str(), _mqtt_config.mqttUsername.c_str(), _mqtt_config.mqttPassword.c_str());
      } else {
        Serial.println("MQTT NOT using authentication");
        result = mqtt_client.connect(clientId.c_str());
      }
      if (result)
      {
        Serial.println("MQTT Connected");
      }
      else
      {
        Serial.print("failed, mqtt state=");
        Serial.print(mqtt_client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
    return true;
  }
  return false;
}

void callback(char* topic, byte* payload, unsigned int length) {
  #ifdef ATH_RELAY
    Relay_MQTT_Received(topic, payload, length);
  #endif
}

PubSubClient MQTT_Setup(String deviceId, StorageValues rootConfig)
{
  _mqtt_deviceId = deviceId;
  _mqtt_config = rootConfig;
  Serial.print("Connecting to MQTT server: ");
  Serial.print(_mqtt_config.mqttServer);
  Serial.print("Connecting to MQTT server PORT: ");
  Serial.print(_mqtt_config.mqttPort);
  mqtt_client.setServer(_mqtt_config.mqttServer.c_str(), _mqtt_config.mqttPort.toInt());

  #ifdef ATH_RELAY
    mqtt_client.setCallback(callback);
  #endif


  return mqtt_client;
}

void MQTT_Loop()
{
  if(ConnectToMqtt()) {
    #ifdef ATH_RELAY
      Serial.println("Setting up MQTT subscriber");
      mqtt_client.subscribe("iot/relay/+");
      Serial.println("Setup MQTT Subscriber");
    #endif
  }
  mqtt_client.loop();
}


#endif
