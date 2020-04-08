//AthosDHT11.cpp
#ifdef ATH_DHT11

// Wiring:
//BMP     -> NodeMCU
//VCC     -> 3V
//GRND    -> G
//SIGNAL  -> D4
//https://www.instructables.com/id/Interface-DHT11-Humidity-Sensor-Using-NodeMCU/

#include "AthosDHT11.h"
#include "AthosHelpers.h"
#include "AthosNTP.h"
#include "AnalogSmooth.h"
#include <ArduinoJson.h>
#include <ArduinoLog.h>
#include <PubSubClient.h>
#include "Wire.h"
#include "DHT.h"

#define DHTPIN D4
#define DHTTYPE DHT11


DHT _dht11_dht = DHT(DHTPIN, DHTTYPE);

int _DHT11_loop_delay = 1000;
PubSubClient _DHT11_mqtt_client;
String _DHT11_deviceId;
StorageValues _DHT11_config;
AthosHelpers _DHT11_helpers;
AthosNTP _DHT11_ntp;

void AthosDHT11::DHT11_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _DHT11_mqtt_client = mqtt_client;
  _DHT11_deviceId = deviceId;
  _DHT11_config = rootConfig;
  _DHT11_loop_delay = loop_delay;

  _dht11_dht.begin();

  Log.trace("Waiting for the DHT11 sensor to come online");
  bool ready = false;
  while (!ready)
  {
    float temperature = _dht11_dht.readTemperature();
    ready = !isnan(temperature);
    delay(100);
  }
  Log.trace("DHT11 sensor is online, proceeding.");
}

void AthosDHT11::DHT11_sendReadingToMQTT(float temp, float humidity, float heatIndex)
{

  long ts = _DHT11_ntp.NTP_getEpochTime();
  String csv = String("DHT11," + _DHT11_helpers.getVersion() + "," + ts + "," + temp + "," + humidity + ","+ heatIndex + "," + _DHT11_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _DHT11_config.mqttSensorTopic.c_str();
  Log.trace("Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!_DHT11_mqtt_client.publish(topic, payload))
  {
    Log.trace("DHT11 Data to MQTT Failed. Packet > 128?");
  }
  _DHT11_helpers.MQTTTransmitLed();
}


AnalogSmooth DHT11_AnalogSmooth_temp = AnalogSmooth(100);
AnalogSmooth DHT11_AnalogSmooth_humid = AnalogSmooth(100);
//max variation in temperature before reporting
//https://cdn-learn.adafruit.com/downloads/pdf/dht.pdf
float DHT11_temp_max_variation = 2;
float DHT11_humid_max_variation = 3;  //5% accuracy

float DHT11_temp_last = 1;
float DHT11_humid_last = 1;

void AthosDHT11::checkAndReportReadings()
{
  // Now we can publish stuff!
  float temperature = DHT11_AnalogSmooth_temp.smooth(_dht11_dht.readTemperature());
  float humidity = DHT11_AnalogSmooth_humid.smooth(_dht11_dht.readHumidity());

  if (isnan(humidity) || isnan(temperature))
  {
    Log.trace(F("Failed to read from DHT sensor!"));
    return;
  }

  float heatIndex = _dht11_dht.computeHeatIndex(temperature, humidity, false);

  bool send = false;

  if (_DHT11_helpers.shouldSend(temperature, DHT11_temp_last, DHT11_temp_max_variation))
  {
    send = true;
    DHT11_temp_last = temperature;
  }
  else if (_DHT11_helpers.shouldSend(humidity, DHT11_humid_last, DHT11_humid_max_variation))
  {
    send = true;
    DHT11_humid_last = humidity;
  }

  if (send)
  {
    DHT11_sendReadingToMQTT(temperature, humidity, heatIndex);
  }
}

void AthosDHT11::DHT11_Loop()
{
  checkAndReportReadings();
}

/*
  Constructor
*/
AthosDHT11::AthosDHT11()
{
}


AthosDHT11::AthosDHT11(AthosHelpers helpers, AthosNTP ntp)
{
  _DHT11_helpers = helpers;
  _DHT11_ntp = ntp;
}
#endif
