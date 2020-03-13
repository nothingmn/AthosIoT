#ifdef ATH_RELAY
#include <ArduinoJson.h>
#include <PubSubClient.h>


// Relay pin is controlled with D8. The active wire is connected to Normally Closed and common
#define RELAY_PIN_D0  D0
#define RELAY_PIN_D1  D1
#define RELAY_PIN_D2  D2
#define RELAY_PIN_D3  D4
#define RELAY_PIN_D4  D5
#define turn_On 1
#define turn_Off 0

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

   // Pin for relay module set as output
  pinMode(RELAY_PIN_D0, OUTPUT);
  digitalWrite(RELAY_PIN_D0, turn_Off);      
  delay(500);
}

void Relay_MQTT_Received(char* topic, byte* payload, unsigned int length) {
  
  payload[length] = '\0';
  String json = String((char*)payload);  
  String strTopic = String(topic);

    Serial.println("mqtt payload received");
    Serial.println(json);
    Serial.println(strTopic);

  if(strTopic.endsWith("/" + _Relay_deviceId)) {
    //matched to the current device.  now do something with the JSON payload;
    StaticJsonDocument<255> readDoc;
    deserializeJson(readDoc, json);
    String command = readDoc["command"].as<String>();
    if(command == "on") {
        digitalWrite(RELAY_PIN_D0, turn_On);      
    } else {
        digitalWrite(RELAY_PIN_D0, turn_Off);      
    }
  }
}


void Relay_Loop()
{
}

#endif
