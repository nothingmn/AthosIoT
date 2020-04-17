#ifdef ATH_MQ2
#ifndef ATH_MQ2_
#define ATH_MQ2_

///Make sure you have a 3V MQ2 Sensor
//MQ2   -> ESP8266
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

PubSubClient _MQ2_mqtt_client;
String _MQ2_deviceId;
StorageValues _MQ2_config;
int _MQ2_loop_delay;

AnalogSmooth MQ2_Ratio_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ2_LPG_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ2_Methane_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ2_Smoke_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ2_Hydrogen_AnalogSmooth = AnalogSmooth(100);

MQ2 mq2(A0);


void MQ2_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _MQ2_mqtt_client = mqtt_client;
  _MQ2_deviceId = deviceId;
  _MQ2_config = rootConfig;
  _MQ2_loop_delay = loop_delay;
  
  mq2.heaterPwrHigh();
  WarmingUpLed();
}


void sendMQ2ToMQTT(float ratio, float lpg, float methane, float smoke, float hydrogen)
{
  long ts = NTP_getEpochTime();

  String csv = String("MQ2," + getVersion() + "," + ts + "," + ratio +"," + lpg + "," + methane +"," + smoke +"," + hydrogen +"," + _MQ2_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _MQ2_config.mqttSensorTopic.c_str();
  Log.trace("MQ2: Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!_MQ2_mqtt_client.publish(topic, payload)) {
    Log.trace("MQ2 Data to MQTT Failed. Packet > 128?");
  }

  MQTTTransmitLed();
}

//https://www.pololu.com/file/0J309/MQ2.pdf
float MQ2_RATIO_TOLERANCE = 1.0;
float MQ2_LPG_TOLERANCE = 1.0;
float MQ2_METHANE_TOLERANCE = 1.0;
float MQ2_SMOKE_TOLERANCE = 1.0;
float MQ2_HYDROGEN_TOLERANCE = 1.0;

float MQ2_RATIO_LAST_REPORTED = 0;
float MQ2_LPG_LAST_REPORTED = 0;
float MQ2_METHANE_LAST_REPORTED = 0;
float MQ2_SMOKE_LAST_REPORTED = 0;
float MQ2_HYDROGEN_LAST_REPORTED = 0;

void MQ2_Loop()
{

    if (!mq2.isCalibrated() && mq2.heatingCompleted()) {
      mq2.calibrate();
      WarmedUpLed();      
    }

    if (mq2.isCalibrated() && mq2.heatingCompleted()) {

      float ratio = MQ2_Ratio_AnalogSmooth.smooth(mq2.readRatio());
      float lpg = MQ2_LPG_AnalogSmooth.smooth(mq2.readLPG());
      float methane = MQ2_Methane_AnalogSmooth.smooth(mq2.readMethane());
      float smoke = MQ2_Smoke_AnalogSmooth.smooth(mq2.readSmoke());
      float hydrogen = MQ2_Hydrogen_AnalogSmooth.smooth(mq2.readHydrogen());

      bool reported = false;
      float diff = abs(ratio - MQ2_RATIO_LAST_REPORTED);
      if(diff > MQ2_RATIO_TOLERANCE) {
        //report
        sendMQ2ToMQTT(ratio, lpg, methane, smoke, hydrogen);
        MQ2_RATIO_LAST_REPORTED = ratio;
        reported = true;
      }

      if(!reported) {    
        diff = abs(lpg - MQ2_LPG_LAST_REPORTED);
        if(diff > MQ2_LPG_TOLERANCE) {
          //report
          sendMQ2ToMQTT(ratio, lpg, methane, smoke, hydrogen);
          MQ2_LPG_LAST_REPORTED = lpg;
          reported = true;
        }
      }

      if(!reported) {    
        diff = abs(methane - MQ2_METHANE_LAST_REPORTED);
        if(diff > MQ2_METHANE_TOLERANCE) {
          //report
          sendMQ2ToMQTT(ratio, lpg, methane, smoke, hydrogen);
          MQ2_METHANE_LAST_REPORTED = methane;
          reported = true;
        }
      }

      if(!reported) {    
        diff = abs(smoke - MQ2_SMOKE_LAST_REPORTED);
        if(diff > MQ2_SMOKE_TOLERANCE) {
          //report
          sendMQ2ToMQTT(ratio, lpg, methane, smoke, hydrogen);
          MQ2_SMOKE_LAST_REPORTED = smoke;
          reported = true;
        }
      }    

      
      if(!reported) {    
        diff = abs(hydrogen - MQ2_HYDROGEN_LAST_REPORTED);
        if(diff > MQ2_HYDROGEN_TOLERANCE) {
          //report
          sendMQ2ToMQTT(ratio, lpg, methane, smoke, hydrogen);
          MQ2_HYDROGEN_LAST_REPORTED = hydrogen;
          reported = true;
        }
      }    
  }

}

#endif
#endif