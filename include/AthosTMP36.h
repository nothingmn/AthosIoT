#ifdef ATH_TMP36
#include <ArduinoJson.h>
#include <PubSubClient.h>


// Wiring:
//TMP     -> NodeMCU
//VC/1    -> 3V
//VOUT/2  -> A0
//GRND/3  -> G
//http://www.esp8266learning.com/wemos-tmp36-example.php

//max variation in temperature before reporting
float temp_max_variation = 1;
//max time between mandatory reporting
float max_time_between = 1000 * 10; //10 seconds

int TMP36_sensorPin = A0;

int _tmp36_loop_delay = 1000;
PubSubClient _tmp36_mqtt_client;
String _tmp36_deviceId;
StorageValues _tmp36_config;



void TMP_sendCapsToMQTT()
{
  Log.trace("TMP_sendCapsToMQTT");
  long ts = NTP_getEpochTime();
  StaticJsonDocument<200> doc;
  doc["caps"]["cap"] = "TMP36";
  doc["caps"]["ts"] = ts;
  doc["deviceid"] = _tmp36_deviceId;
  String json;
  serializeJson(doc, json);
  Log.trace(json.c_str());
  Log.trace(_tmp36_config.mqttCapsTopic.c_str());
  _tmp36_mqtt_client.publish(_tmp36_config.mqttCapsTopic.c_str(), json.c_str());
  MQTTTransmitLed();
}


void TMP_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
    _tmp36_mqtt_client = mqtt_client;
    _tmp36_deviceId = deviceId;
    _tmp36_config = rootConfig;
    _tmp36_loop_delay = loop_delay;

    TMP_sendCapsToMQTT();
}

float TMP36_readTemperature() {
    int reading = analogRead(TMP36_sensorPin);
    // measure the 3.3v with a meter for an accurate value
    //In particular if your Arduino is USB powered
    float voltage = reading * 3.3;
    voltage /= 1024.0;

    // now print out the temperature
    return (voltage - 0.5) * 100;
}


float readTemperature()
{
  int sensorPin = 0;
  int reading = analogRead(sensorPin);
  // measure the 3.3v with a meter for an accurate value
  //In particular if your Arduino is USB powered
  float voltage = reading * 3.3;
  voltage /= 1024.0;

  // now print out the temperature
  return (voltage - 0.5) * 100;
}

void sendTemperatureToMQTT(float value)
{
  long ts = NTP_getEpochTime();

  StaticJsonDocument<200> doc;
  doc["temp"] = value;
  doc["ts"] = ts;
  doc["deviceid"] = _tmp36_deviceId;
  String json;
  serializeJson(doc, json);
  Log.trace(json.c_str());
  

  if (_tmp36_mqtt_client.publish(_tmp36_config.mqttSensorTopic.c_str(), json.c_str())) {
      Log.trace(F("Failed"));
  }

  MQTTTransmitLed();
}

float last_recorded_temp = 0;
float time_since_last = 0;

void checkAndReportReadings()
{
  // Now we can publish stuff!
  float temperature = readTemperature();
  float diff = temperature - last_recorded_temp;
  if (abs(diff) > temp_max_variation || time_since_last > max_time_between)
  {
    sendTemperatureToMQTT(temperature);
    last_recorded_temp = temperature;
    time_since_last = 0;
  }
  else
  {
    time_since_last += _tmp36_loop_delay;
  }
}



void TMP_Loop()
{
    checkAndReportReadings();
}

#endif
