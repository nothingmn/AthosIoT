#ifdef ATH_MQ8
#ifndef ATH_MQ8_
#define ATH_MQ8_
//MQ-8 - Hydrogen Gas    
///Make sure you have a 3V MQ8 Sensor
//MQ8   -> ESP8266
//A0    ->  A0
//D1    ->  NOT USED
//GRND  ->  GRND
//VCC   ->  VCC

#include "Arduino.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <TroykaMQ.h>

#ifndef AnalogSmooth_h
  #include "AnalogSmooth.h"
  #include "AnalogSmooth.cpp"
#endif

PubSubClient _MQ8_mqtt_client;
String _MQ8_deviceId;
StorageValues _MQ8_config;
int _MQ8_loop_delay;

MQ8 MQ8(A0);


void MQ8_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _MQ8_mqtt_client = mqtt_client;
  _MQ8_deviceId = deviceId;
  _MQ8_config = rootConfig;
  _MQ8_loop_delay = loop_delay;
  
  MQ8.heaterPwrHigh();
  WarmingUpLed();
}


void sendMQ8ToMQTT(float ratio, float HYDROGEN)
{
  long ts = NTP_getEpochTime();

  String csv = String("MQ8," + getVersion() + "," + ts + "," + ratio + "," + HYDROGEN + "," + _MQ8_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _MQ8_config.mqttSensorTopic.c_str();
  Log.trace("MQ8: Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!_MQ8_mqtt_client.publish(topic, payload)) {
    Log.trace("MQ8 Data to MQTT Failed. Packet > 128?");
  }

  MQTTTransmitLed();
}



AnalogSmooth MQ8_Ratio_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ8_HYDROGEN_AnalogSmooth = AnalogSmooth(100);

float MQ8_RATIO_TOLERANCE = 1.0;
float MQ8_HYDROGEN_TOLERANCE = 1.0;

float MQ8_RATIO_LAST_REPORTED = 0;
float MQ8_HYDROGEN_LAST_REPORTED = 0;

void MQ8_Loop()
{

    if (!MQ8.isCalibrated() && MQ8.heatingCompleted()) {
      MQ8.calibrate();
      Log.trace("Ro = %f", MQ8.getRo());
      WarmedUpLed();      
    }

    if (MQ8.isCalibrated() && MQ8.heatingCompleted()) {

      float ratio = MQ8_Ratio_AnalogSmooth.smooth(MQ8.readRatio());
      float HYDROGEN = MQ8_HYDROGEN_AnalogSmooth.smooth(MQ8.readHydrogen());


      bool reported = false;
      float diff = abs(ratio - MQ8_RATIO_LAST_REPORTED);
      if(diff > MQ8_RATIO_TOLERANCE) {
        //report
        sendMQ8ToMQTT(ratio, HYDROGEN);
        MQ8_RATIO_LAST_REPORTED = ratio;
        reported = true;
      }

      if(!reported) {    
        diff = abs(HYDROGEN - MQ8_HYDROGEN_LAST_REPORTED);
        if(diff > MQ8_HYDROGEN_TOLERANCE) {
          //report
          sendMQ8ToMQTT(ratio, HYDROGEN);
          MQ8_HYDROGEN_LAST_REPORTED = HYDROGEN;
          reported = true;
        }
      }
  }

}

#endif
#endif