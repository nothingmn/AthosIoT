#ifdef ATH_BUTTON

#ifndef ATH_BUTTON_
#define ATH_BUTTON_
#include <string>
#include <PubSubClient.h>
#include <ArduinoLog.h>


//https://www.instructables.com/id/Arduino-Button-with-no-resistor/

const int ButtonPin = D0;

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
   pinMode(ButtonPin, INPUT_PULLUP);
}

void BUTTON_Loop()
{
   // Read the value of the input. It can either be 1 or 0
   int buttonValue = digitalRead(ButtonPin);

   if (buttonValue == LOW){
      Log.trace("button pressed");
   } else {
      Log.trace("button NOT pressed");
   }
   
    delay(1000);
}




#endif
#endif