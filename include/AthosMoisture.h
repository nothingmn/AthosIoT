#ifdef ATH_MOIST
#ifndef ATH_MOIST_
#define ATH_MOIST_

///Make sure you have a 3V MOIST Sensor
///In this sketch, we are using a A0 to measure the resistance over the moisture sensor

#include "Arduino.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>

#ifndef AnalogSmooth_h
  #include "AnalogSmooth.h"
  #include "AnalogSmooth.cpp"
#endif

PubSubClient _MOIST_mqtt_client;
String _MOIST_deviceId;
StorageValues _MOIST_config;
int _MOIST_loop_delay;

int MOIST_inputPin = A0; // choose the input pin (for moisture sensor)
AnalogSmooth MOIST_AnalogSmooth = AnalogSmooth(100);

void MOIST_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _MOIST_mqtt_client = mqtt_client;
  _MOIST_deviceId = deviceId;
  _MOIST_config = rootConfig;
  _MOIST_loop_delay = loop_delay;

}


void sendMoistureToMQTT(long moisture)
{
  if(moisture > 100)  moisture = 100;
  if(moisture < 0)  moisture = 0;

  long ts = NTP_getEpochTime();

  String csv = String("MOIST," + getVersion() + "," + ts + "," + moisture + "," + _MOIST_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _MOIST_config.mqttSensorTopic.c_str();
  Log.trace("Moisture: Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!_MOIST_mqtt_client.publish(topic, payload)) {
    Log.trace("MOIST Data to MQTT Failed. Packet > 128?");
  }

  MQTTTransmitLed();
}

int MOIST_MIN = 575;
int MOIST_MAX = 1024;
float MOISTURE_TOLERANCE = 2.0;
float MOISTURE_LAST_REPORTED = 0;
void MOIST_Loop()
{
    //test out your sensor, get an idea of your min and max ranges by testing in and out of water (not fizzy water)
    //int value = analogRead(MOIST_inputPin);
    // float smoothed = MOIST_AnalogSmooth.smooth(value * 1.0);    
    // Serial.printf("Value:%i, Smoothed:%f", value, smoothed);

    //720 to 1024 is the range i found for my sensor
    //as usual we are going to do a moving average of our readings over 100 readings
    float smoothed = MOIST_AnalogSmooth.analogReadSmooth(MOIST_inputPin);    

    //We will use the map function to map the values read on the analog input range from 0 to 1023, and will be remapped to another range from 0 to 100%.
    long moisture = map ( smoothed, MOIST_MAX, MOIST_MIN, 0, 100 );
    float diff = abs(moisture - MOISTURE_LAST_REPORTED);
    if(diff > MOISTURE_TOLERANCE) {
      //report
      sendMoistureToMQTT(moisture);
      MOISTURE_LAST_REPORTED = moisture;
    }

}

#endif
#endif