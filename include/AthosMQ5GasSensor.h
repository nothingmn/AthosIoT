#ifdef ATH_MQ5
#ifndef ATH_MQ5_
#define ATH_MQ5_
//MQ-5 - Natural gas, LPG    
///Make sure you have a 3V MQ5 Sensor
//MQ5   -> ESP8266
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

PubSubClient _MQ5_mqtt_client;
String _MQ5_deviceId;
StorageValues _MQ5_config;
int _MQ5_loop_delay;

MQ5 MQ5(A0);


void MQ5_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _MQ5_mqtt_client = mqtt_client;
  _MQ5_deviceId = deviceId;
  _MQ5_config = rootConfig;
  _MQ5_loop_delay = loop_delay;
  
  MQ5.heaterPwrHigh();
  WarmingUpLed();
}


void sendMQ5ToMQTT(float ratio, float LPG, float methane)
{
  long ts = NTP_getEpochTime();

  String csv = String("MQ5," + getVersion() + "," + ts + "," + ratio + "," + LPG + ","+ methane + "," + _MQ5_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _MQ5_config.mqttSensorTopic.c_str();
  Log.trace("MQ5: Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!_MQ5_mqtt_client.publish(topic, payload)) {
    Log.trace("MQ5 Data to MQTT Failed. Packet > 128?");
  }

  MQTTTransmitLed();
}



AnalogSmooth MQ5_Ratio_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ5_LPG_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ5_methane_AnalogSmooth = AnalogSmooth(100);

float MQ5_RATIO_TOLERANCE = 1.0;
float MQ5_LPG_TOLERANCE = 1.0;
float MQ5_methane_TOLERANCE = 1.0;

float MQ5_RATIO_LAST_REPORTED = 0;
float MQ5_LPG_LAST_REPORTED = 0;
float MQ5_methane_LAST_REPORTED = 0;

void MQ5_Loop()
{

    if (!MQ5.isCalibrated() && MQ5.heatingCompleted()) {
      MQ5.calibrate();
      Log.trace("Ro = %f", MQ5.getRo());
      WarmedUpLed();      
    }

    if (MQ5.isCalibrated() && MQ5.heatingCompleted()) {

      float ratio = MQ5_Ratio_AnalogSmooth.smooth(MQ5.readRatio());
      float LPG = MQ5_LPG_AnalogSmooth.smooth(MQ5.readLPG());
      float methane = MQ5_LPG_AnalogSmooth.smooth(MQ5.readMethane());

      bool reported = false;
      float diff = abs(ratio - MQ5_RATIO_LAST_REPORTED);
      if(diff > MQ5_RATIO_TOLERANCE) {
        //report
        sendMQ5ToMQTT(ratio, LPG, methane);
        MQ5_RATIO_LAST_REPORTED = ratio;
        reported = true;
      }

      if(!reported) {    
        diff = abs(LPG - MQ5_LPG_LAST_REPORTED);
        if(diff > MQ5_LPG_TOLERANCE) {
          //report
          sendMQ5ToMQTT(ratio, LPG, methane);
          MQ5_LPG_LAST_REPORTED = LPG;
          reported = true;
        }
      }


      if(!reported) {    
        diff = abs(methane - MQ5_methane_LAST_REPORTED);
        if(diff > MQ5_methane_TOLERANCE) {
          //report
          sendMQ5ToMQTT(ratio, LPG, methane);
          MQ5_methane_LAST_REPORTED = LPG;
          reported = true;
        }
      }
  }

}

#endif
#endif