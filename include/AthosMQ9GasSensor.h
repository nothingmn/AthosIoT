#ifdef ATH_MQ9
#ifndef ATH_MQ9_
#define ATH_MQ9_
//MQ-9 - Carbon Monoxide, flammable gasses   
///Make sure you have a 3V MQ9 Sensor
//MQ9   -> ESP8266
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

PubSubClient _MQ9_mqtt_client;
String _MQ9_deviceId;
StorageValues _MQ9_config;
int _MQ9_loop_delay;

MQ9 MQ9(A0);


void MQ9_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _MQ9_mqtt_client = mqtt_client;
  _MQ9_deviceId = deviceId;
  _MQ9_config = rootConfig;
  _MQ9_loop_delay = loop_delay;
  
  MQ9.cycleHeat();
  WarmingUpLed();
}


void sendMQ9ToMQTT(float ratio, float LPG, float methane, float co)
{
  long ts = NTP_getEpochTime();

  String csv = String("MQ9," + getVersion() + "," + ts + "," + ratio + "," + LPG + ","+ methane + ","+ co + "," + _MQ9_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _MQ9_config.mqttSensorTopic.c_str();
  Log.trace("MQ9: Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!_MQ9_mqtt_client.publish(topic, payload)) {
    Log.trace("MQ9 Data to MQTT Failed. Packet > 128?");
  }

  MQTTTransmitLed();
}



AnalogSmooth MQ9_Ratio_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ9_LPG_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ9_Methane_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ9_CO_AnalogSmooth = AnalogSmooth(100);

float MQ9_RATIO_TOLERANCE = 1.0;
float MQ9_LPG_TOLERANCE = 1.0;
float MQ9_Methane_TOLERANCE = 1.0;
float MQ9_CO_TOLERANCE = 1.0;

float MQ9_RATIO_LAST_REPORTED = 0;
float MQ9_LPG_LAST_REPORTED = 0;
float MQ9_METHANE_LAST_REPORTED = 0;
float MQ9_CO_LAST_REPORTED = 0;

void MQ9_Loop()
{

    if (!MQ9.isCalibrated() && MQ9.atHeatCycleEnd()) {
      MQ9.calibrate();
      Serial.print("Ro = ");
      Serial.println(MQ9.getRo());
      MQ9.cycleHeat();
      WarmedUpLed();      
    }

    if (MQ9.isCalibrated() && MQ9.atHeatCycleEnd()) {

      float ratio = MQ9_Ratio_AnalogSmooth.smooth(MQ9.readRatio());
      float LPG = MQ9_LPG_AnalogSmooth.smooth(MQ9.readLPG());
      float methane = MQ9_LPG_AnalogSmooth.smooth(MQ9.readMethane());
      float co = MQ9_LPG_AnalogSmooth.smooth(MQ9.readCarbonMonoxide());


      bool reported = false;
      float diff = abs(ratio - MQ9_RATIO_LAST_REPORTED);
      if(diff > MQ9_RATIO_TOLERANCE) {
        //report
        sendMQ9ToMQTT(ratio, LPG, methane, co);
        MQ9_RATIO_LAST_REPORTED = ratio;
        reported = true;
      }

      if(!reported) {    
        diff = abs(LPG - MQ9_LPG_LAST_REPORTED);
        if(diff > MQ9_LPG_TOLERANCE) {
          //report
          sendMQ9ToMQTT(ratio, LPG, methane, co);
          MQ9_LPG_LAST_REPORTED = LPG;
          reported = true;
        }
      }


      if(!reported) {    
        diff = abs(methane - MQ9_METHANE_LAST_REPORTED);
        if(diff > MQ9_Methane_TOLERANCE) {
          //report
          sendMQ9ToMQTT(ratio, LPG, methane, co);
          MQ9_METHANE_LAST_REPORTED = LPG;
          reported = true;
        }
      }



      if(!reported) {    
        diff = abs(LPG - MQ9_CO_LAST_REPORTED);
        if(diff > MQ9_CO_TOLERANCE) {
          //report
          sendMQ9ToMQTT(ratio, LPG, methane, co);
          MQ9_CO_LAST_REPORTED = LPG;
          reported = true;
        }
      }

      MQ9.cycleHeat();
  }

}

#endif
#endif