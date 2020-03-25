#ifdef ATH_BMP280

// Wiring:
//BMP  -> NodeMCU
//VCC  -> 3V
//GRND -> G
//SCL  -> D1
//SDA  -> D2
// https://randomnerdtutorials.com/esp8266-bme280-arduino-ide/
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
uint8_t i2cAddress = 0x76;

PubSubClient _BMP280_mqtt_client;
String _BMP280_deviceId;
StorageValues _BMP280_config;



void BMP280_sendCapsToMQTT()
{
  log_info("BMP280_sendCapsToMQTT");
  long ts = NTP_getEpochTime();
  StaticJsonDocument<200> doc;
  doc["caps"]["cap"] = "BMP280";
  doc["caps"]["ts"] = ts;
  doc["deviceid"] = _BMP280_deviceId;
  String json;
  serializeJson(doc, json);
  log_info(json)
  log_info(_BMP280_config.mqttCapsTopic.c_str())
  _BMP280_mqtt_client.publish(_BMP280_config.mqttCapsTopic.c_str(), json.c_str());
  MQTTTransmitLed();
}


void BMP280_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _BMP280_mqtt_client = mqtt_client;
  _BMP280_deviceId = deviceId;
  _BMP280_config = rootConfig;
  _BMP280_loop_delay = loop_delay;
  if (!bme.begin(i2cAddress))
  {
    log_info("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    log_info("SensorID was: 0x");
    log_info(bme.sensorID(), 16);
    log_info("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085");
    log_info("   ID of 0x56-0x58 represents a BMP 280,");
    log_info("        ID of 0x60 represents a BME 280.");
    log_info("        ID of 0x61 represents a BME 680.");
  }
  BMP280_sendCapsToMQTT();
}

void sendReadingToMQTT(float temp, float humidity, float pressure, float altitude)
{

  long ts = NTP_getEpochTime();

  StaticJsonDocument<128> doc;
  doc.clear();
  doc["ts"] = ts;
  doc["deviceid"]  = _BMP280_deviceId.c_str();
  doc["temp"]  = temp;
  doc["hum"]  = humidity;
  doc["press"]  = pressure;
  doc["alt"]  = altitude;
  
  String json;
  serializeJson(doc, json);
  log_info(json);
  log_info(json.length());
  log_info(_BMP280_config.mqttSensorTopic);
  
  if (!_BMP280_mqtt_client.publish(_BMP280_config.mqttSensorTopic.c_str(), json.c_str()))
  {
    log_info(F("BMP20 Data to MQTT Failed"));
    //_BMP280_mqtt_client.disconnect();
  } else {
    MQTTTransmitLed();
  }
  doc.clear();
}

float last_recorded_temp = 0;
float time_since_last = 0;

void checkAndReportReadings()
{
  // Now we can publish stuff!
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure();
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  if (isnan(humidity) || isnan(temperature) || isnan(pressure))
  {
    log_info(F("Failed to read from BMP sensor!"));
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

