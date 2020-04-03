#ifdef ATH_DHT11

// Wiring:
//BMP     -> NodeMCU
//VCC     -> 3V
//GRND    -> G
//SIGNAL  -> D4
//https://www.instructables.com/id/Interface-DHT11-Humidity-Sensor-Using-NodeMCU/

#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "Wire.h"
#include "DHT.h"
#define DHTPIN D4
#define DHTTYPE DHT11

#ifndef AnalogSmooth_h
#include "AnalogSmooth.h"
#include "AnalogSmooth.cpp"
#endif

//Try to avoid having anything in global scope

DHT dht(DHTPIN, DHTTYPE);
int _DHT11_loop_delay = 1000;
PubSubClient _DHT11_mqtt_client;
String _DHT11_deviceId;
StorageValues _DHT11_config;

void DHT11_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _DHT11_mqtt_client = mqtt_client;
  _DHT11_deviceId = deviceId;
  _DHT11_config = rootConfig;
  _DHT11_loop_delay = loop_delay;

  dht.begin();

  Log.trace("Waiting for the DHT11 sensor to come online");
  bool ready = false;
  while (!ready)
  {
    float temperature = dht.readTemperature();
    ready = !isnan(temperature);
    delay(100);
  }
  Log.trace("DHT11 sensor is online, proceeding.");
}

void DHT11_sendReadingToMQTT(float temp, float humidity, float heatIndex)
{

  long ts = NTP_getEpochTime();

  StaticJsonDocument<200> doc;
  doc["ts"] = ts;
  doc["temp"] = temp;
  doc["humidity"] = humidity;
  doc["heatIndex"] = heatIndex;
  doc["deviceid"] = _DHT11_deviceId;
  doc["v"] = getVersion();
  doc["b"] = getBuild();
  
  String json;
  serializeJson(doc, json);
  Log.trace(json.c_str());
  if (!_DHT11_mqtt_client.publish(_DHT11_config.mqttSensorTopic.c_str(), json.c_str()), false)
  {
    Log.trace(F("Failed"));
  }
  MQTTTransmitLed();
}


AnalogSmooth DHT11_AnalogSmooth_temp = AnalogSmooth(100);
AnalogSmooth DHT11_AnalogSmooth_humid = AnalogSmooth(100);
//max variation in temperature before reporting
//https://cdn-learn.adafruit.com/downloads/pdf/dht.pdf
float DHT11_temp_max_variation = 2;
float DHT11_humid_max_variation = 3;  //5% accuracy

float DHT11_temp_last = 1;
float DHT11_humid_last = 1;

void checkAndReportReadings()
{
  // Now we can publish stuff!
  float temperature = DHT11_AnalogSmooth_temp.smooth(dht.readTemperature());
  float humidity = DHT11_AnalogSmooth_humid.smooth(dht.readHumidity());

  if (isnan(humidity) || isnan(temperature))
  {
    Log.trace(F("Failed to read from DHT sensor!"));
    return;
  }

  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);

  bool send = false;

  if (shouldSend(temperature, DHT11_temp_last, DHT11_temp_max_variation))
  {
    send = true;
    DHT11_temp_last = temperature;
  }
  else if (shouldSend(humidity, DHT11_humid_last, DHT11_humid_max_variation))
  {
    send = true;
    DHT11_humid_last = humidity;
  }

  if (send)
  {
    DHT11_sendReadingToMQTT(temperature, humidity, heatIndex);
  }
}

void DHT11_Loop()
{
  checkAndReportReadings();
}

#endif
