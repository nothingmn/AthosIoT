#ifdef ATH_MQ135
#ifndef ATH_MQ135_
#define ATH_MQ135_

///Make sure you have a 3V MQ135 Sensor
//MQ135   -> ESP8266
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

PubSubClient _MQ135_mqtt_client;
String _MQ135_deviceId;
StorageValues _MQ135_config;
int _MQ135_loop_delay;

MQ135 MQ135(A0);


void MQ135_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _MQ135_mqtt_client = mqtt_client;
  _MQ135_deviceId = deviceId;
  _MQ135_config = rootConfig;
  _MQ135_loop_delay = loop_delay;
  
  MQ135.heaterPwrHigh();
  WarmingUpLed();
}


void sendMQ135ToMQTT(float ratio, float airQuality)
{
  long ts = NTP_getEpochTime();

  String csv = String("MQ135," + getVersion() + "," + ts + "," + ratio +"," + airQuality + "," + _MQ135_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _MQ135_config.mqttSensorTopic.c_str();
  Log.trace("MQ135: Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!_MQ135_mqtt_client.publish(topic, payload)) {
    Log.trace("MQ135 Data to MQTT Failed. Packet > 128?");
  }

  MQTTTransmitLed();
}



AnalogSmooth MQ135_Ratio_AnalogSmooth = AnalogSmooth(100);
AnalogSmooth MQ135_AirQuality_AnalogSmooth = AnalogSmooth(100);

float MQ135_RATIO_TOLERANCE = 1.0;
float MQ135_AirQuality_TOLERANCE = 1.0;

float MQ135_RATIO_LAST_REPORTED = 0;
float MQ135_AirQuality_LAST_REPORTED = 0;

void MQ135_Loop()
{

    if (!MQ135.isCalibrated() && MQ135.heatingCompleted()) {
      MQ135.calibrate();
      WarmedUpLed();      
    }

    if (MQ135.isCalibrated() && MQ135.heatingCompleted()) {

      float ratio = MQ135_Ratio_AnalogSmooth.smooth(MQ135.readRatio());
      float airQuality = MQ135_AirQuality_AnalogSmooth.smooth(MQ135.readCO2());
      bool reported = false;
      float diff = abs(ratio - MQ135_RATIO_LAST_REPORTED);
      if(diff > MQ135_RATIO_TOLERANCE) {
        //report
        sendMQ135ToMQTT(ratio, airQuality);
        MQ135_RATIO_LAST_REPORTED = ratio;
        reported = true;
      }

      if(!reported) {    
        diff = abs(airQuality - MQ135_AirQuality_LAST_REPORTED);
        if(diff > MQ135_AirQuality_TOLERANCE) {
          //report
          sendMQ135ToMQTT(ratio, airQuality);
          MQ135_AirQuality_LAST_REPORTED = airQuality;
          reported = true;
        }
      }
  }

}

#endif
#endif