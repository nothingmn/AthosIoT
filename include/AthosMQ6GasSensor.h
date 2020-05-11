#ifdef ATH_MQ6
#ifndef ATH_MQ6_
#define ATH_MQ6_
//MQ-6 - LPG, butane    
///Make sure you have a 3V MQ6 Sensor
//MQ6   -> ESP8266
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

PubSubClient _MQ6_mqtt_client;
String _MQ6_deviceId;
StorageValues _MQ6_config;
int _MQ6_loop_delay;

MQ6 MQ6(A0);


void MQ6_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _MQ6_mqtt_client = mqtt_client;
  _MQ6_deviceId = deviceId;
  _MQ6_config = rootConfig;
  _MQ6_loop_delay = loop_delay;
  
  MQ6.heaterPwrHigh();
  WarmingUpLed();
}


void sendMQ6ToMQTT(float ratio, float LPG, float butane)
{
  long ts = NTP_getEpochTime();

  String csv = String("MQ6," + getVersion() + "," + ts + "," + ratio + "," + LPG + ","+ butane + "," + _MQ6_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _MQ6_config.mqttSensorTopic.c_str();
  Log.trace("MQ6: Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!_MQ6_mqtt_client.publish(topic, payload)) {
    Log.trace("MQ6 Data to MQTT Failed. Packet > 128?");
  }

  MQTTTransmitLed();
}



AnalogSmooth MQ6_Ratio_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ6_LPG_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ6_butane_AnalogSmooth = AnalogSmooth(100);

float MQ6_RATIO_TOLERANCE = 1.0;
float MQ6_LPG_TOLERANCE = 1.0;
float MQ6_butane_TOLERANCE = 1.0;

float MQ6_RATIO_LAST_REPORTED = 0;
float MQ6_LPG_LAST_REPORTED = 0;
float MQ6_butane_LAST_REPORTED = 0;

void MQ6_Loop()
{

    if (!MQ6.isCalibrated() && MQ6.heatingCompleted()) {
      MQ6.calibrate();
      Log.trace("Ro = ");
      Log.trace(MQ6.getRo());
      WarmedUpLed();      
    }

    if (MQ6.isCalibrated() && MQ6.heatingCompleted()) {

      float ratio = MQ6_Ratio_AnalogSmooth.smooth(MQ6.readRatio());
      float LPG = MQ6_LPG_AnalogSmooth.smooth(MQ6.readLPG());
      float butane = MQ6_LPG_AnalogSmooth.smooth(0);
      //float butane = MQ6_LPG_AnalogSmooth.smooth(MQ6.readButane());

      bool reported = false;
      float diff = abs(ratio - MQ6_RATIO_LAST_REPORTED);
      if(diff > MQ6_RATIO_TOLERANCE) {
        //report
        sendMQ6ToMQTT(ratio, LPG, butane);
        MQ6_RATIO_LAST_REPORTED = ratio;
        reported = true;
      }

      if(!reported) {    
        diff = abs(LPG - MQ6_LPG_LAST_REPORTED);
        if(diff > MQ6_LPG_TOLERANCE) {
          //report
          sendMQ6ToMQTT(ratio, LPG, butane);
          MQ6_LPG_LAST_REPORTED = LPG;
          reported = true;
        }
      }


      if(!reported) {    
        diff = abs(butane - MQ6_butane_LAST_REPORTED);
        if(diff > MQ6_butane_TOLERANCE) {
          //report
          sendMQ6ToMQTT(ratio, LPG, butane);
          MQ6_butane_LAST_REPORTED = LPG;
          reported = true;
        }
      }
  }

}

#endif
#endif