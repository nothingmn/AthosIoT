#ifdef ATH_SIMPLE_BUTTON

#ifndef ATH_SIMPLE_BUTTON_
#define ATH_SIMPLE_BUTTON_
#include <string>
#include <PubSubClient.h>
#include <ArduinoLog.h>
#include <Arduino.h>


//https://www.instructables.com/id/Arduino-Button-with-no-resistor/
//https://forum.arduino.cc/index.php?topic=72276.msg541217#msg541217



const uint SIMPLE_BUTTON_PINS[] {D0,D1,D2};
String SIMPLE_BUTTON_Report_PINS[] {"D0","D1","D2"};

int SIMPLE_BUTTON_States[sizeof(SIMPLE_BUTTON_PINS)];
int SIMPLE_BUTTON_COUNT = sizeof( SIMPLE_BUTTON_PINS ) / sizeof( SIMPLE_BUTTON_PINS[0] );

PubSubClient _SIMPLE_BUTTON_mqtt_client;
String _SIMPLE_BUTTON_deviceId;
StorageValues _SIMPLE_BUTTON_config;
int _SIMPLE_BUTTON_loop_delay = 1000;

void SIMPLE_BUTTON_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _SIMPLE_BUTTON_mqtt_client = mqtt_client;
  _SIMPLE_BUTTON_deviceId = deviceId;
  _SIMPLE_BUTTON_config = rootConfig;
  _SIMPLE_BUTTON_loop_delay = loop_delay;

   // Define pin D0 as input and activate the internal pull-up resistor
   for(int x=0;x<SIMPLE_BUTTON_COUNT;x++) {
      Log.trace("Setting up pin:%i (%i), %s", SIMPLE_BUTTON_PINS[x], x,SIMPLE_BUTTON_Report_PINS[x].c_str());
      pinMode(SIMPLE_BUTTON_PINS[x], INPUT);
      digitalWrite(pinNumber, HIGH);
      SIMPLE_BUTTON_States[x] = HIGH;
   }
}

void SendButtonStatesToMqtt() {
  long ts = NTP_getEpochTime();
  String value = "";
   for(int x=0;x<SIMPLE_BUTTON_COUNT;x++) {
      value += SIMPLE_BUTTON_Report_PINS[x] + ":" + SIMPLE_BUTTON_States[x] + ",";
   }
  String csv = String("BUT," + getVersion() + "," + ts + "," + value + _SIMPLE_BUTTON_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _SIMPLE_BUTTON_config.mqttSensorTopic.c_str();
  Log.trace("Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!_SIMPLE_BUTTON_mqtt_client.publish(topic, payload)) {
    Log.trace("BUTTON Data to MQTT Failed. Packet > 128?");
  }

  MQTTTransmitLed();
}

void ReadButtonStates() {
   bool send = false;
   for(int x=0;x<SIMPLE_BUTTON_COUNT;x++) {  
      int buttonValue = digitalRead(SIMPLE_BUTTON_PINS[x]);
      int last = SIMPLE_BUTTON_States[x];
      if(last != buttonValue) {
         if (buttonValue == LOW){
            Log.trace("button pressed DOWN:%s", SIMPLE_BUTTON_Report_PINS[x].c_str());
         } else {
            Log.trace("button UP:%s", SIMPLE_BUTTON_Report_PINS[x].c_str());
         }
         SIMPLE_BUTTON_States[x] = buttonValue;
         send = true;
      }
   }
   if(send) SendButtonStatesToMqtt();
}

void SIMPLE_BUTTON_Loop()
{
   ReadButtonStates();
}




#endif
#endif