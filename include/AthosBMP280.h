#ifdef ATH_BMP280

#ifndef ATH_BMP280_
#define ATH_BMP280_

// Wiring:
//BMP  -> NodeMCU
//VCC  -> 3V
//GRND -> G
//SCL  -> D1
//SDA  -> D2
// https://randomnerdtutorials.com/esp8266-bme280-arduino-ide/
// https://github.com/adafruit/Adafruit_BME280_Library/blob/master/examples/bme280test/bme280test.ino

#include <string>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#ifndef AnalogSmooth_h
  #include "AnalogSmooth.h"
  #include "AnalogSmooth.cpp"
#endif

#define SEALEVELPRESSURE_HPA (1013.25)


Adafruit_BME280 bme; // I2C
uint8_t i2cAddress = 0x76;

PubSubClient _BMP280_mqtt_client;
String _BMP280_deviceId;
StorageValues _BMP280_config;
int _BMP280_loop_delay = 1000;

AnalogSmooth BMP_AnalogSmooth_temp = AnalogSmooth(100);
AnalogSmooth BMP_AnalogSmooth_humid = AnalogSmooth(100);
AnalogSmooth BMP_AnalogSmooth_press = AnalogSmooth(100);
AnalogSmooth BMP_AnalogSmooth_alt = AnalogSmooth(100);


void BMP280_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _BMP280_mqtt_client = mqtt_client;
  _BMP280_deviceId = deviceId;
  _BMP280_config = rootConfig;
  _BMP280_loop_delay = loop_delay;
  bool errorShown = false;

  int max_wait = 60; //60 seconds
  int wait = 0;
  while (!bme.begin(i2cAddress))
  {
    wait++;
    if(wait > max_wait) ESP.restart();
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

void sendReadingToMQTT(float temp, float humidity, float pressure, float altitude)
{

  long ts = NTP_getEpochTime();
  String csv = String("BMP280," + getVersion() + "," + ts + "," + temp + "," + humidity + ","+ pressure + ","+ altitude + "," + _BMP280_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _BMP280_config.mqttSensorTopic.c_str();
  Log.trace("Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());
  
  if (!_BMP280_mqtt_client.publish(topic, payload))
  {    
    Log.trace("BMP20 Data to MQTT Failed. Packet > 128?");
  } else {
    MQTTTransmitLed();
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


void BMP280_checkAndReportReadings()
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

  if(shouldSend(temperature, _BMP280_last_recorded_temp, _BMP280_temp_max_variation)) {
    send = true;
    _BMP280_last_recorded_temp = temperature;
  } else if(shouldSend(humidity, _BMP280_last_recorded_humid, _BMP280_humid_max_variation)) {
    send = true;
    _BMP280_last_recorded_humid = humidity;
  } else if(shouldSend(pressure, _BMP280_last_recorded_press, _BMP280_press_max_variation)) {
    send = true;
    _BMP280_last_recorded_press = pressure;
  } 

  if(send) {
        sendReadingToMQTT(temperature, humidity, pressure, altitude);
  }
}

void BMP280_Loop()
{
  BMP280_checkAndReportReadings();
}




#endif
#endif