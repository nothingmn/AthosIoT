#ifdef ATH_RELAY

#ifndef ATH_RELAY_
#define ATH_RELAY_

#include <AthosHelpers.h>
#include <AthosNTP.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <ArduinoLog.h>

#ifdef ATH_MQTT
  #include "AthosMQTT.h"
#endif 


#ifdef ATH_RELAY_MD0
  uint RELAY_PINS[] {D0,D1};
  String RELAY_Report_PINS = "MD0,MD1";
  long duration = 1000;  //if > 0 it will trigger back given the duraiton; emulates a momentary switch
#else
  //add or remove and sort items from the array to control the pins as you like.
  uint RELAY_PINS[] {D0};
  //uint RELAY_PINS[] {D0,D1};
  //change this so we can report up to to the server
  String RELAY_Report_PINS = "D0";
  //String RELAY_Report_PINS = "MD0,MD1";
  long duration = 0;  //if > 0 it will trigger back given the duraiton; emulates a momentary switch
#endif

int RELAY_COUNT = sizeof( RELAY_PINS ) / sizeof( RELAY_PINS[0] );


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
  for(int x=0;x<RELAY_COUNT;x++) {
    pinMode(RELAY_PINS[x], OUTPUT);
    digitalWrite(RELAY_PINS[x], turn_On);
    if(duration>0) {
      delay(duration);
      digitalWrite(RELAY_PINS[x], turn_Off);
    }
  }
}

bool Relay_MQTT_Received(String topic, String json) {
  
  if(topic.endsWith("/" + _Relay_deviceId)) {

    Log.trace("Relay_MQTT_Received  \nTopic:%s\nPayload:%s", topic.c_str(), json.c_str());
    
    //matched to the current device.  now do something with the JSON payload;
    StaticJsonDocument<255> readDoc;
    deserializeJson(readDoc, json);
    String command = readDoc["command"].as<String>();

    if(command == "on" || command == "off") {
      int vpin = readDoc["pin"].as<int>();
      int pin = RELAY_PINS[0];

      Log.trace("Relay_MQTT_Received:\npin index:%i\npin:%i\ncommand:%s", vpin, pin, command.c_str());

      if(vpin <= sizeof(RELAY_PINS)) {
        pin = RELAY_PINS[vpin];
      }

      if(command == "on") {
          digitalWrite(pin, turn_On);      
          Log.trace("on");
          if(duration > 0) {
            delay(duration);
            digitalWrite(pin, turn_Off);      
            Log.trace("off");
          }
          return true;
      } else {
          digitalWrite(pin, turn_Off);      
          Log.trace("off");
          if(duration > 0) {
            delay(duration);
            digitalWrite(pin, turn_On);      
            Log.trace("on");
          }
          return true;
      }
    }
    if(command == "allon") {
      for(int x=0;x<sizeof(RELAY_PINS);x++) {
        digitalWrite(RELAY_PINS[x], turn_On);      
        if(duration > 0) {
          delay(duration);
          digitalWrite(RELAY_PINS[x], turn_Off);      
        }
      }

      return true;
    }
    if(command == "alloff") {
      for(int x=0;x<sizeof(RELAY_PINS);x++) {
        digitalWrite(RELAY_PINS[x], turn_Off);      
        if(duration > 0) {
          delay(duration);
          digitalWrite(RELAY_PINS[x], turn_On);      
        }
      }
      return true;
    }
  }
  return false;
}

void RELAY_CheckIn()
{
  Log.trace("RELAY_CheckIn");
  int ts = NTP_getEpochTime();
  
  String csv = String("RELAY," + getVersion() + "," + ts + "," + RELAY_Report_PINS + "," +  _Relay_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _Relay_config.mqttSensorTopic.c_str();
  Serial.printf("Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());
  Log.trace("Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!_Relay_mqtt_client.publish(topic, payload))
  {
    Log.trace("RELAY Checkin Data to MQTT Failed. Packet > 128?");
  }
  MQTTTransmitLed();
}

long relay_last = 0;
long relay_max_diff = 60;
void Relay_Loop()
{
  long now = NTP_getEpochTime();
  long diff = abs(now - relay_last);  
  if(diff > relay_max_diff || relay_last == 0) {
    relay_last = now;
    RELAY_CheckIn();
  }
}

#endif
#endif