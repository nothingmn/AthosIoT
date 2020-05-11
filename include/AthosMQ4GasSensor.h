#ifdef ATH_MQ4
#ifndef ATH_MQ4_
#define ATH_MQ4_
// MQ-4 - Methane, CNG Gas    ** Library only supports methane at the moment.
///Make sure you have a 3V MQ4 Sensor
//MQ4   -> ESP8266
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

PubSubClient _MQ4_mqtt_client;
String _MQ4_deviceId;
StorageValues _MQ4_config;
int _MQ4_loop_delay;

MQ4 MQ4(A0);


void MQ4_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _MQ4_mqtt_client = mqtt_client;
  _MQ4_deviceId = deviceId;
  _MQ4_config = rootConfig;
  _MQ4_loop_delay = loop_delay;
  
  MQ4.heaterPwrHigh();
  WarmingUpLed();
}


void sendMQ4ToMQTT(float ratio, float methane, float cng)
{
  long ts = NTP_getEpochTime();

  String csv = String("MQ4," + getVersion() + "," + ts + "," + ratio + "," + methane + ","+ cng + "," + _MQ4_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _MQ4_config.mqttSensorTopic.c_str();
  Log.trace("MQ4: Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!_MQ4_mqtt_client.publish(topic, payload)) {
    Log.trace("MQ4 Data to MQTT Failed. Packet > 128?");
  }

  MQTTTransmitLed();
}



AnalogSmooth MQ4_Ratio_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ4_CNG_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ4_methane_AnalogSmooth = AnalogSmooth(100);

float MQ4_RATIO_TOLERANCE = 1.0;
float MQ4_CNG_TOLERANCE = 1.0;
float MQ4_methane_TOLERANCE = 1.0;

float MQ4_RATIO_LAST_REPORTED = 0;
float MQ4_CNG_LAST_REPORTED = 0;
float MQ4_methane_LAST_REPORTED = 0;

void MQ4_Loop()
{

    if (!MQ4.isCalibrated() && MQ4.heatingCompleted()) {
      MQ4.calibrate();
      Log.trace("Ro = ");
      Log.trace(MQ4.getRo());
      WarmedUpLed();      
    }

    if (MQ4.isCalibrated() && MQ4.heatingCompleted()) {

      float ratio = MQ4_Ratio_AnalogSmooth.smooth(MQ4.readRatio());
      float CNG = 0;//MQ4_CNG _AnalogSmooth.smooth(MQ4.readLPG());
      float methane = MQ4_CNG_AnalogSmooth.smooth(MQ4.readMethane());

      bool reported = false;
      float diff = abs(ratio - MQ4_RATIO_LAST_REPORTED);
      if(diff > MQ4_RATIO_TOLERANCE) {
        //report
        sendMQ4ToMQTT(ratio, CNG, methane);
        MQ4_RATIO_LAST_REPORTED = ratio;
        reported = true;
      }

      if(!reported) {    
        diff = abs(CNG - MQ4_CNG_LAST_REPORTED);
        if(diff > MQ4_CNG_TOLERANCE) {
          //report
          sendMQ4ToMQTT(ratio, CNG, methane);
          MQ4_CNG_LAST_REPORTED = CNG;
          reported = true;
        }
      }


      if(!reported) {    
        diff = abs(methane - MQ4_methane_LAST_REPORTED);
        if(diff > MQ4_methane_TOLERANCE) {
          //report
          sendMQ4ToMQTT(ratio, CNG, methane);
          MQ4_methane_LAST_REPORTED = methane;
          reported = true;
        }
      }
  }

}

#endif
#endif