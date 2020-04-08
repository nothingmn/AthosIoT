//AthosBMP280.cpp
#ifdef ATH_BMP280

// Wiring:
//BMP  -> NodeMCU
//VCC  -> 3V
//GRND -> G
//SCL  -> D1
//SDA  -> D2
// https://randomnerdtutorials.com/esp8266-bme280-arduino-ide/
// https://github.com/adafruit/Adafruit_BME280_Library/blob/master/examples/bme280test/bme280test.ino

#include "AthosBMP280.h"
#include "AnalogSmooth.h"
#include "AthosHelpers.h"
#include "AthosNTP.h"

#include <string>
#include <ArduinoLog.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

//max time between mandatory reporting
int _BMP280_loop_delay = 1000;

Adafruit_BME280 bme; // I2C
uint8_t i2cAddress = 0x76;

PubSubClient _BMP280_mqtt_client;
String _BMP280_deviceId;
StorageValues _BMP280_config;

AnalogSmooth BMP_AnalogSmooth_temp = AnalogSmooth(100);
AnalogSmooth BMP_AnalogSmooth_humid = AnalogSmooth(100);
AnalogSmooth BMP_AnalogSmooth_press = AnalogSmooth(100);
AnalogSmooth BMP_AnalogSmooth_alt = AnalogSmooth(100);

AthosHelpers _bmp_helpers;
AthosNTP _bmp_ntp;

void AthosBMP280::BMP280_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _BMP280_mqtt_client = mqtt_client;
  _BMP280_deviceId = deviceId;
  _BMP280_config = rootConfig;
  _BMP280_loop_delay = loop_delay;
  bool errorShown = false;
  while (!bme.begin(i2cAddress))
  {
    if(!errorShown) {
      Log.trace("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
      Log.trace("SensorID was: %i %i", bme.sensorID());
      Log.trace("   ID of 0xFF probably means a bad address, a BMP 180 or BMP 085");
      Log.trace("   ID of 0x56-0x58 represents a BMP 280,");
      Log.trace("   ID of 0x60 represents a BME 280.");
      Log.trace("   ID of 0x61 represents a BME 680.");
      errorShown = true;  
      Log.trace("Waiting for BME280 sensor...");
    }  
    Log.trace(".");
    delay(1000);
  }
}

void AthosBMP280::sendReadingToMQTT(float temp, float humidity, float pressure, float altitude)
{

  long ts = _bmp_ntp.NTP_getEpochTime();
  String csv = String("BMP280," + _bmp_helpers.getVersion() + "," + ts + "," + temp + "," + humidity + ","+ pressure + ","+ altitude + "," + _BMP280_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _BMP280_config.mqttSensorTopic.c_str();
  Log.trace("Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());
  
  if (!_BMP280_mqtt_client.publish(topic, payload))
  {    
    Log.trace("BMP20 Data to MQTT Failed. Packet > 128?");
  } else {
    _bmp_helpers.MQTTTransmitLed();
  }
}

float _BMP280_last_recorded_temp = 0;
float _BMP280_last_recorded_humid = 0;
float _BMP280_last_recorded_press = 0;
float _BMP280_last_recorded_alt = 0;

//https://www.bosch-sensortec.com/media/boschsensortec/downloads/environmental_sensors_2/humidity_sensors_1/bme280/bst-bme280-ds002.pdf
float _BMP280_temp_max_variation = 1.0;
float _BMP280_humid_max_variation = 3.0;
float _BMP280_press_max_variation = 1.7;


void AthosBMP280::BMP280_checkAndReportReadings(void)
{
  // Now we can publish stuff!
  float temperature = BMP_AnalogSmooth_temp.smooth(bme.readTemperature());
  float humidity = BMP_AnalogSmooth_humid.smooth(bme.readHumidity());
  float pressure =BMP_AnalogSmooth_press.smooth( bme.readPressure());
  float altitude = BMP_AnalogSmooth_alt.smooth(bme.readAltitude(SEALEVELPRESSURE_HPA));

  if (isnan(humidity) || isnan(temperature) || isnan(pressure))
  {
    Log.trace(F("Failed to read from BMP sensor!"));
    return;
  }

  bool send = false;

  if(_bmp_helpers.shouldSend(temperature, _BMP280_last_recorded_temp, _BMP280_temp_max_variation)) {
    send = true;
    _BMP280_last_recorded_temp = temperature;
  } else if(_bmp_helpers.shouldSend(humidity, _BMP280_last_recorded_humid, _BMP280_humid_max_variation)) {
    send = true;
    _BMP280_last_recorded_humid = humidity;
  } else if(_bmp_helpers.shouldSend(pressure, _BMP280_last_recorded_press, _BMP280_press_max_variation)) {
    send = true;
    _BMP280_last_recorded_press = pressure;
  } 

  if(send) {
        sendReadingToMQTT(temperature, humidity, pressure, altitude);
  }
}

void AthosBMP280::BMP280_Loop(void)
{
  BMP280_checkAndReportReadings();
}


/*
  Constructor
*/
AthosBMP280::AthosBMP280()
{
}

AthosBMP280::AthosBMP280(AthosHelpers helpers, AthosNTP ntp)
{
  _bmp_helpers = helpers;
  _bmp_ntp = ntp;
}

#endif

