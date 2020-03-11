#ifdef ATH_BMP280
//https://learn.adafruit.com/adafruit-bme280-humidity-barometric-pressure-temperature-sensor-breakout?view=all
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Adafruit_BME280.h>

// #define BME_SCK 13
// #define BME_MISO 12
// #define BME_MOSI 11 
// #define BME_CS 10
 

//max variation in temperature before reporting
float temp_max_variation = 1;
//max time between mandatory reporting
float max_time_between = 1000 * 10; //10 seconds

int _BMP280_loop_delay = 1000;
//Try to avoid having anything in global scope

Adafruit_BME280 bme; // I2C

PubSubClient _BMP280_mqtt_client;
String _BMP280_deviceId;
StorageValues _BMP280_config;

void BMP280_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
    _BMP280_mqtt_client = mqtt_client;
    _BMP280_deviceId = deviceId;
    _BMP280_config = rootConfig;
    _BMP280_loop_delay = loop_delay;

    if (!bme.begin()) {  
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
    }
}

void sendReadingToMQTT(float temp, float humidity, float pressure) {
  
  long ts = NTP_getEpochTime();

  StaticJsonDocument<200> doc;
  doc["ts"] = ts;
  doc["temp"] = temp;
  doc["humidity"] = humidity;
  doc["pressure"] = pressure;
  doc["deviceid"] = _BMP280_deviceId;
  String json;
  serializeJson(doc, json);
  Serial.println(json);
  if (! _BMP280_mqtt_client.publish(_BMP280_config.mqttSensorTopic.c_str(), json.c_str())) {
      Serial.println(F("Failed"));
  }
  BlinkLed();
}


float last_recorded_temp = 0;
float time_since_last = 0;

void checkAndReportReadings()
{
  // Now we can publish stuff!
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readHumidity();

  if (isnan(humidity) || isnan(temperature) || isnan(pressure)) {
    Serial.println(F("Failed to read from BMP sensor!"));
    return;
  }

  float diff = temperature - last_recorded_temp;
  if (abs(diff) > temp_max_variation || time_since_last > max_time_between)
  {
    sendReadingToMQTT(temperature, humidity, pressure);
    last_recorded_temp = temperature;
    time_since_last = 0;
  }
  else
  {
    time_since_last += _BMP280_loop_delay;
  }
}



void BMP280_Loop()
{
    checkAndReportReadings();
}

#endif
