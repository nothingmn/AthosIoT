#ifdef ATH_BMP280
//https://github.com/adafruit/Adafruit_BME280_Library/blob/master/examples/bme280test/bme280test.ino
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

//Try to avoid having anything in global scope

//max variation in temperature before reporting
float _BMP280_temp_max_variation = 1;
//max time between mandatory reporting
float _BMP280_max_time_between = 1000 * 10; //10 seconds
int _BMP280_loop_delay = 1000;

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

    if (!bme.begin(0x76)) {  
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
    }
}

void sendReadingToMQTT(float temp, float humidity, float pressure, float altitude) {
  
  long ts = NTP_getEpochTime();

  StaticJsonDocument<200> doc;
  doc["ts"] = ts;
  doc["temp"] = temp;
  doc["humidity"] = humidity;
  doc["pressure"] = pressure;
  doc["altitude"] = altitude;
  doc["deviceid"] = _BMP280_deviceId;
  String json;
  serializeJson(doc, json);
  Serial.println(json);
  if (! _BMP280_mqtt_client.publish(_BMP280_config.mqttSensorTopic.c_str(), json.c_str())) {
      Serial.println(F("Failed"));
  }
  MQTTTransmitLed();
}


void printValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  // Convert temperature to Fahrenheit
  /*Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");*/
  
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}

float last_recorded_temp = 0;
float time_since_last = 0;

void checkAndReportReadings()
{
  printValues();
  // Now we can publish stuff!
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure();
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  if (isnan(humidity) || isnan(temperature) || isnan(pressure)) {
    Serial.println(F("Failed to read from BMP sensor!"));
    return;
  }

  float diff = abs(temperature - last_recorded_temp);
  if (diff > _BMP280_temp_max_variation || time_since_last > _BMP280_max_time_between)
  {
    sendReadingToMQTT(temperature, humidity, pressure, altitude);
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
