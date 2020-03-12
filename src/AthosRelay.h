#ifdef ATH_RELAY
#include <ArduinoJson.h>
#include <PubSubClient.h>

PubSubClient _Relay_mqtt_client;
String _Relay_deviceId;
StorageValues _Relay_config;
int _Relay_loop_delay;

void Relay_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _Relay_mqtt_client = mqtt_client;
  _Relay_deviceId = deviceId;
  _Relay_config = rootConfig;
  _Relay_loop_delay = loop_delay;

}

void Relay_MQTT_Received(char* topic, byte* payload, unsigned int length) {
  
  payload[length] = '\0';
  String json = String((char*)payload);  
  String strTopic = String(topic);

  if(strTopic.endsWith("/" + _Relay_deviceId)) {
    //matched to the current device.  now do something with the JSON payload;
    StaticJsonDocument<255> readDoc;
    deserializeJson(readDoc, json);
    String command = readDoc["command"].as<String>();

    if(command == "on") {
      Relay_ON_Led();
    } else {
      Relay_OFF_Led();
    }
  }
}



void Relay_Loop()
{
}

#endif
