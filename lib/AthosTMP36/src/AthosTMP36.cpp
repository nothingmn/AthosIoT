//AthosTMP36.cpp

#ifdef ATH_TMP36
#include "AthosTMP36.h"
#include "AnalogSmooth.h"
#include "AthosHelpers.h"
#include "AthosNTP.h"
#include <ArduinoLog.h>
#include <PubSubClient.h>

// Wiring:
//TMP     -> NodeMCU
//VC/1    -> 3V
//VOUT/2  -> A0
//GRND/3  -> G
//http://www.esp8266learning.com/wemos-tmp36-example.php

//max variation in temperature before reporting
//https://www.analog.com/media/en/technical-documentation/data-sheets/TMP35_36_37.pdf
//±2°C accuracy over temperature (typ)
float temp_max_variation = 2.0; 

int TMP36_sensorPin = A0;

int _tmp36_loop_delay = 1000;
PubSubClient _tmp36_mqtt_client;
String _tmp36_deviceId;
StorageValues _tmp36_config;

AnalogSmooth TMP_AnalogSmooth = AnalogSmooth(100);
AthosHelpers _tmp36_helpers;
AthosNTP _tmp36_ntp;

void AthosTMP36::TMP_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
    _tmp36_mqtt_client = mqtt_client;
    _tmp36_deviceId = deviceId;
    _tmp36_config = rootConfig;
    _tmp36_loop_delay = loop_delay;

}

float AthosTMP36::TMP36_readTemperature(void) {
    int reading = analogRead(TMP36_sensorPin);
    // measure the 3.3v with a meter for an accurate value
    //In particular if your Arduino is USB powered
    float voltage = reading * 3.3;
    voltage /= 1024.0;

    float temp = (voltage - 0.5) * 100;

    float smoothed = TMP_AnalogSmooth.smooth(temp);
    return smoothed;
}

void AthosTMP36::sendTemperatureToMQTT(float value, float diff)
{
  long ts = _tmp36_ntp.NTP_getEpochTime();
  String csv = String("TMP36," + _tmp36_helpers.getVersion() + "," + ts + "," + value + "," + diff + "," + _tmp36_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _tmp36_config.mqttSensorTopic.c_str();
  bool connected = _tmp36_mqtt_client.connected();

  Log.trace("\nConnected:%i\nTopic:%s\nPayload:%s\nLength:%i\n",connected?1:0, topic, payload, csv.length());


  if (!_tmp36_mqtt_client.publish(topic, payload)) {
    Log.trace("TMP36 Data to MQTT Failed. Packet > 128?");
  }

  _tmp36_helpers.MQTTTransmitLed();
}

float last_recorded_temp = 0;
float time_since_last = 0;

void AthosTMP36::checkAndReportReadings(void)
{
  // Now we can publish stuff!
  float temperature = TMP36_readTemperature();
  float diff = temperature - last_recorded_temp;
  if (abs(diff) > temp_max_variation)
  {
    sendTemperatureToMQTT(temperature, diff);
    last_recorded_temp = temperature;
  }
}



void AthosTMP36::TMP_Loop(void)
{
    checkAndReportReadings();
}

/*
  Constructor
*/
AthosTMP36::AthosTMP36()
{
}

AthosTMP36::AthosTMP36(AthosHelpers helpers, AthosNTP ntp)
{
  _tmp36_helpers = helpers;
  _tmp36_ntp = ntp;
}
#endif
