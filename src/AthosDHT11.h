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

    Serial.println("Waiting for the DHT11 sensor to come online");
    bool ready = false;
    while(!ready){
      float temperature = dht.readTemperature();
      ready = !isnan(temperature);
      delay(100);
    }
    Serial.println("DHT11 sensor is online, proceeding.");
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
  if (! _DHT11_mqtt_client.publish(_DHT11_config.mqttSensorTopic.c_str(), json.c_str()), false) {
      Serial.println(F("Failed"));
  }
  MQTTTransmitLed();
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
