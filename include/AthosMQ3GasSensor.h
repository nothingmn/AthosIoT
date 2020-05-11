#ifdef ATH_MQ3
#ifndef ATH_MQ3_
#define ATH_MQ3_
// MQ-3 - Alcohol, Ethanol, Smoke       ** Library only supports Alcohol at the moment.
///Make sure you have a 3V MQ3 Sensor
//MQ3   -> ESP8266
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

PubSubClient _MQ3_mqtt_client;
String _MQ3_deviceId;
StorageValues _MQ3_config;
int _MQ3_loop_delay;

MQ3 MQ3(A0);


void MQ3_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _MQ3_mqtt_client = mqtt_client;
  _MQ3_deviceId = deviceId;
  _MQ3_config = rootConfig;
  _MQ3_loop_delay = loop_delay;
  
  MQ3.heaterPwrHigh();
  WarmingUpLed();
}


void sendMQ3ToMQTT(float ratio, float AlcoholPPM, float AlcoholMGL)
{
  long ts = NTP_getEpochTime();

  String csv = String("MQ3," + getVersion() + "," + ts + "," + ratio + "," + AlcoholPPM + ","+ AlcoholMGL + "," + _MQ3_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _MQ3_config.mqttSensorTopic.c_str();
  Log.trace("MQ3: Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!_MQ3_mqtt_client.publish(topic, payload)) {
    Log.trace("MQ3 Data to MQTT Failed. Packet > 128?");
  }

  MQTTTransmitLed();
}



AnalogSmooth MQ3_Ratio_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ3_AlcoholMGL_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ3_AlcoholPPM_AnalogSmooth = AnalogSmooth(100);

float MQ3_RATIO_TOLERANCE = 1.0;
float MQ3_AlcoholMGL_TOLERANCE = 1.0;
float MQ3_AlcoholPPM_TOLERANCE = 1.0;

float MQ3_RATIO_LAST_REPORTED = 0;
float MQ3_AlcoholMGL_LAST_REPORTED = 0;
float MQ3_AlcoholPPM_LAST_REPORTED = 0;

void MQ3_Loop()
{

    if (!MQ3.isCalibrated() && MQ3.heatingCompleted()) {
      MQ3.calibrate();
      Log.trace("Ro = ");
      Log.trace(MQ3.getRo());
      WarmedUpLed();      
    }

    if (MQ3.isCalibrated() && MQ3.heatingCompleted()) {

      float ratio = MQ3_Ratio_AnalogSmooth.smooth(MQ3.readRatio());
      float AlcoholMGL = MQ3_AlcoholMGL_AnalogSmooth.smooth(MQ3.readAlcoholMgL());
      float AlcoholPPM = MQ3_AlcoholPPM_AnalogSmooth.smooth(MQ3.readAlcoholPpm());

      bool reported = false;
      float diff = abs(ratio - MQ3_RATIO_LAST_REPORTED);
      if(diff > MQ3_RATIO_TOLERANCE) {
        //report
        sendMQ3ToMQTT(ratio, AlcoholMGL, AlcoholPPM);
        MQ3_RATIO_LAST_REPORTED = ratio;
        reported = true;
      }

      if(!reported) {    
        diff = abs(AlcoholMGL - MQ3_AlcoholMGL_LAST_REPORTED);
        if(diff > MQ3_AlcoholMGL_TOLERANCE) {
          //report
          sendMQ3ToMQTT(ratio, AlcoholMGL, AlcoholPPM);
          MQ3_AlcoholMGL_LAST_REPORTED = AlcoholMGL;
          reported = true;
        }
      }


      if(!reported) {    
        diff = abs(AlcoholPPM - MQ3_AlcoholPPM_LAST_REPORTED);
        if(diff > MQ3_AlcoholPPM_TOLERANCE) {
          //report
          sendMQ3ToMQTT(ratio, AlcoholMGL, AlcoholPPM);
          MQ3_AlcoholPPM_LAST_REPORTED = AlcoholPPM;
          reported = true;
        }
      }
  }

}

#endif
#endif