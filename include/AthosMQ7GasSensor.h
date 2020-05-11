#ifdef ATH_MQ7
#ifndef ATH_MQ7_
#define ATH_MQ7_
//MQ-7 - Carbon Monoxide    
///Make sure you have a 3V MQ7 Sensor
//MQ7   -> ESP8266
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

PubSubClient _MQ7_mqtt_client;
String _MQ7_deviceId;
StorageValues _MQ7_config;
int _MQ7_loop_delay;

MQ7 MQ7(A0);


void MQ7_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _MQ7_mqtt_client = mqtt_client;
  _MQ7_deviceId = deviceId;
  _MQ7_config = rootConfig;
  _MQ7_loop_delay = loop_delay;
  
  MQ7.cycleHeat();
  WarmingUpLed();
}


void sendMQ7ToMQTT(float ratio, float co)
{
  long ts = NTP_getEpochTime();

  String csv = String("MQ7," + getVersion() + "," + ts + "," + ratio + "," + co + "," + _MQ7_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _MQ7_config.mqttSensorTopic.c_str();
  Log.trace("MQ7: Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!_MQ7_mqtt_client.publish(topic, payload)) {
    Log.trace("MQ7 Data to MQTT Failed. Packet > 128?");
  }

  MQTTTransmitLed();
}



AnalogSmooth MQ7_Ratio_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ7_CO_AnalogSmooth = AnalogSmooth(100);

float MQ7_RATIO_TOLERANCE = 1.0;
float MQ7_CO_TOLERANCE = 1.0;

float MQ7_RATIO_LAST_REPORTED = 0;
float MQ7_CO_LAST_REPORTED = 0;

void MQ7_Loop()
{

    if (!MQ7.isCalibrated() && MQ7.atHeatCycleEnd()) {
      MQ7.calibrate();
      Log.trace("Ro = ");
      Log.trace(MQ7.getRo());
      MQ7.cycleHeat();
      WarmedUpLed();      
    }

    if (MQ7.isCalibrated() && MQ7.atHeatCycleEnd()) {

      float ratio = MQ7_Ratio_AnalogSmooth.smooth(MQ7.readRatio());
      float co = MQ7_CO_AnalogSmooth.smooth(MQ7.readCarbonMonoxide());


      bool reported = false;
      float diff = abs(ratio - MQ7_RATIO_LAST_REPORTED);
      if(diff > MQ7_RATIO_TOLERANCE) {
        //report
        sendMQ7ToMQTT(ratio, co);
        MQ7_RATIO_LAST_REPORTED = ratio;
        reported = true;
      }

      if(!reported) {    
        diff = abs(co - MQ7_CO_LAST_REPORTED);
        if(diff > MQ7_CO_TOLERANCE) {
          //report
          sendMQ7ToMQTT(ratio,  co);
          MQ7_CO_LAST_REPORTED = co;
          reported = true;
        }
      }

      MQ7.cycleHeat();
  }

}

#endif
#endif