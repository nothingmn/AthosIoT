#ifdef ATH_DHT11

#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "Wire.h"
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11

//max variation in temperature before reporting
float temp_max_variation = 1;
//max time between mandatory reporting
float max_time_between = 1000 * 10; //10 seconds

int _DHT11_loop_delay = 1000;
//Try to avoid having anything in global scope

DHT dht(DHTPIN, DHTTYPE);

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
}

void sendReadingToMQTT(float temp, float humidity, float heatIndex) {
  
  long ts = NTP_getEpochTime();

  StaticJsonDocument<200> doc;
  doc["ts"] = ts;
  doc["temp"] = temp;
  doc["humidity"] = humidity;
  doc["heatIndex"] = heatIndex;
  doc["deviceid"] = _DHT11_deviceId;
  String json;
  serializeJson(doc, json);
  Serial.println(json);
  if (! _DHT11_mqtt_client.publish(_DHT11_config.mqttSensorTopic.c_str(), json.c_str())) {
      Serial.println(F("Failed"));
  }
  BlinkLed();
}


float last_recorded_temp = 0;
float time_since_last = 0;

void checkAndReportReadings()
{
  // Now we can publish stuff!
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);

  float diff = temperature - last_recorded_temp;
  if (abs(diff) > temp_max_variation || time_since_last > max_time_between)
  {
    sendReadingToMQTT(temperature, humidity, heatIndex);
    last_recorded_temp = temperature;
    time_since_last = 0;
  }
  else
  {
    time_since_last += _DHT11_loop_delay;
  }
}



void DHT11_Loop()
{
    checkAndReportReadings();
}

#endif
