#ifdef ATH_BUTTON

#ifndef ATH_BUTTON_
#define ATH_BUTTON_
#include <string>
#include <PubSubClient.h>
#include <ArduinoLog.h>
#include <Arduino.h>


//https://www.instructables.com/id/Arduino-Button-with-no-resistor/

//PS - I should have said don't use digital pin 0, 1 or 13. Any other pin will do.
//Do not use D3, D7, D10

const uint BUTTON_PINS[] {D2};
String BUTTON_Report_PINS[] {"D2"};
int BUTTON_COUNT = sizeof( BUTTON_PINS ) / sizeof( BUTTON_PINS[0] );
int BUTTON_States[sizeof( BUTTON_PINS ) / sizeof( BUTTON_PINS[0] )];

PubSubClient _BUTTON_mqtt_client;
String _BUTTON_deviceId;
StorageValues _BUTTON_config;
int _BUTTON_loop_delay = 1000;

void BUTTON_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _BUTTON_mqtt_client = mqtt_client;
  _BUTTON_deviceId = deviceId;
  _BUTTON_config = rootConfig;
  _BUTTON_loop_delay = loop_delay;

   // Define pin D0 as input and activate the internal pull-up resistor
   for(int x=0;x<BUTTON_COUNT;x++) {
      Log.trace("Setting up pin:%i (%i), %s", BUTTON_PINS[x], x,BUTTON_Report_PINS[x].c_str());
      pinMode(BUTTON_PINS[x], INPUT);
      digitalWrite(BUTTON_PINS[x], HIGH);
      BUTTON_States[x] = HIGH;
   }
}

void SendButtonStatesToMqtt() {
  long ts = NTP_getEpochTime();
  String value = "";
   for(int x=0;x<BUTTON_COUNT;x++) {
      value += BUTTON_Report_PINS[x] + ":" + BUTTON_States[x] + ","; 
   }
  String csv = String("BUT," + getVersion() + "," + ts + "," + value + _BUTTON_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _BUTTON_config.mqttSensorTopic.c_str();
  Log.trace("Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!_BUTTON_mqtt_client.publish(topic, payload)) {
    Log.trace("BUTTON Data to MQTT Failed. Packet > 128?");
  }

  MQTTTransmitLed();
}

void ReadButtonStates() {
   bool send = false;
   for(int x=0;x<BUTTON_COUNT;x++) {  
      int buttonValue = digitalRead(BUTTON_PINS[x]);
      int last = BUTTON_States[x];
      //then if the button is push-to-make, a digitalRead from that pin will return 
      //LOW when the button is pressed, 
      //HIGH when it is not pressed. 
      //If the pushbutton is press-to-break, this will be reversed.
      Log.trace("button=%i, last=%i", x, last);
      if(last != buttonValue) {
         if (buttonValue == LOW){
            Log.trace("button pressed DOWN:%s", BUTTON_Report_PINS[x].c_str());
         } else {
            Log.trace("button UP:%s", BUTTON_Report_PINS[x].c_str());
         }
         BUTTON_States[x] = buttonValue;
         send = true;
      }
   }
   if(send) SendButtonStatesToMqtt();
}

void BUTTON_Loop()
{
   ReadButtonStates();
}




#endif
#endif