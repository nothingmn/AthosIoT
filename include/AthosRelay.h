#ifdef ATH_RELAY
#include <ArduinoJson.h>
#include <PubSubClient.h>


//{"command":"on","pin":0}
// "pin" in that JSON document will match the index of the RELAY_PINS array
//add or remove and sort items from the array to control the pins as you like.
//uint RELAY_PINS[] {};
uint RELAY_PINS[] {D0};
//uint RELAY_PINS[6] {D0, D1, D2, D3, D4, D5};

#define turn_On 1
#define turn_Off 0

PubSubClient _Relay_mqtt_client;
String _Relay_deviceId;
StorageValues _Relay_config;
int _Relay_loop_delay;

void RELAY_sendCapsToMQTT()
{
  log_info("RELAY_sendCapsToMQTT");
  long ts = NTP_getEpochTime();
  StaticJsonDocument<200> doc;
  doc["caps"]["cap"] = "RELAY";
  doc["caps"]["channels"] = "[\"D0\"]";
  doc["caps"]["ts"] = ts;
  doc["deviceid"] = _Relay_deviceId;
  String json;
  serializeJson(doc, json);
  log_info(json);
  log_info(_Relay_config.mqttCapsTopic.c_str());
  _Relay_mqtt_client.publish(_Relay_config.mqttCapsTopic.c_str(), json.c_str());
  MQTTTransmitLed();
}



void Relay_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _Relay_mqtt_client = mqtt_client;
  _Relay_deviceId = deviceId;
  _Relay_config = rootConfig;
  _Relay_loop_delay = loop_delay;

   // Pin for relay module set as output
  for(int x=0;x<sizeof(RELAY_PINS);x++) {
    pinMode(RELAY_PINS[x], OUTPUT);
    digitalWrite(RELAY_PINS[x], turn_Off);
  }
  RELAY_sendCapsToMQTT();
}


void RunFun() {
  for(int x=0;x<=5;x++) {
    for(int y=0;y<sizeof(RELAY_PINS);x++) {
      digitalWrite(RELAY_PINS[y], turn_Off);      
    }
    delay(100);
    for(int z=0;z<sizeof(RELAY_PINS);z++){
      digitalWrite(RELAY_PINS[z], turn_On);      
      delay(100);
      digitalWrite(RELAY_PINS[z], turn_Off);      
      delay(100);
    }
  }
}
bool Relay_MQTT_Received(string topic, string json) {
  
  if(strTopic.endsWith("/" + _Relay_deviceId)) {
    //matched to the current device.  now do something with the JSON payload;
    StaticJsonDocument<255> readDoc;
    deserializeJson(readDoc, json);
    String command = readDoc["command"].as<String>();

    if(command == "on" || command == "off") {
      int vpin = readDoc["pin"].as<int>();
      int pin = RELAY_PINS[0];
      if(vpin <= sizeof(RELAY_PINS)) {
        pin = RELAY_PINS[vpin];
      }

      if(command == "on") {
          digitalWrite(pin, turn_On);      
          return true;
      } else {
          digitalWrite(pin, turn_Off);      
          return true;
      }
    }
    if(command == "allon") {
      for(int x=0;x<sizeof(RELAY_PINS);x++) {
        digitalWrite(RELAY_PINS[x], turn_On);      
      }
      return true;
    }
    if(command == "alloff") {
      for(int x=0;x<sizeof(RELAY_PINS);x++) {
        digitalWrite(RELAY_PINS[x], turn_Off);      
      }
      return true;
    }
    if(command == "fun") {
      RunFun();
      return true;
    }
  }
  return false;
}


void Relay_Loop()
{
}

#endif
