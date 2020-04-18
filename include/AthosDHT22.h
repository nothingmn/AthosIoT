#ifdef ATH_DHT22

#ifndef ATH_DHT22_
#define ATH_DHT22_

// Wiring:
//BMP     -> NodeMCU
//VCC     -> 3V
//GRND    -> G
//SIGNAL  -> D4

//https://learn.adafruit.com/dht?view=all
// DHT wiring is very simple:
//     The left-most pin is power. We recommend powering from 5V (sometimes 3V is not enough) - this is OK even if you are using 3.3V logic
//     The second pin is data. Connect a 10K pullup resistor from this pin to 3.3V. If you are using a DHT11 it's required. If you're using a DHT22 or AM2302 you can sometimes leave this off
//     Skip the third pin
//     The right-most pin is ground

#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "Wire.h"
#include "DHT.h"
#define DHTPIN D4
#define DHTTYPE DHT22

#ifndef AnalogSmooth_h
#include "AnalogSmooth.h"
#include "AnalogSmooth.cpp"
#endif

//Try to avoid having anything in global scope

DHT dht22(DHTPIN, DHTTYPE);
int _DHT22_loop_delay = 1000;
PubSubClient _DHT22_mqtt_client;
String _DHT22_deviceId;
StorageValues _DHT22_config;

void DHT22_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _DHT22_mqtt_client = mqtt_client;
  _DHT22_deviceId = deviceId;
  _DHT22_config = rootConfig;
  _DHT22_loop_delay = loop_delay;

  dht22.begin();

}

void DHT22_sendReadingToMQTT(float temp, float humidity, float heatIndex)
{

  long ts = NTP_getEpochTime();
  String csv = String("DHT22," + getVersion() + "," + ts + "," + temp + "," + humidity + ","+ heatIndex + "," + _DHT22_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _DHT22_config.mqttSensorTopic.c_str();
  Log.trace("Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!_DHT22_mqtt_client.publish(topic, payload))
  {
    Log.trace("DHT22 Data to MQTT Failed. Packet > 128?");
  }
  MQTTTransmitLed();
}


AnalogSmooth DHT22_AnalogSmooth_temp = AnalogSmooth(100);
AnalogSmooth DHT22_AnalogSmooth_humid = AnalogSmooth(100);
//max variation in temperature before reporting
//https://cdn-learn.adafruit.com/downloads/pdf/dht.pdf
float DHT22_temp_max_variation = 0.5;  //Good for -40 to 80°C temperature readings ±0.5°C accuracy
float DHT22_humid_max_variation = 2;  //Good for 0-100% humidity readings with 2-5% accuracy

float DHT22_temp_last = 1;
float DHT22_humid_last = 1;

void checkAndReportReadings()
{
  // Now we can publish stuff!
  float temperature = DHT22_AnalogSmooth_temp.smooth(dht22.readTemperature());
  float humidity = DHT22_AnalogSmooth_humid.smooth(dht22.readHumidity());

  if (isnan(humidity) || isnan(temperature))
  {
    Log.trace(F("Failed to read from DHT sensor!"));
    return;
  }

  float heatIndex = dht22.computeHeatIndex(temperature, humidity, false);

  bool send = false;

  if (shouldSend(temperature, DHT22_temp_last, DHT22_temp_max_variation))
  {
    send = true;
    DHT22_temp_last = temperature;
  }
  else if (shouldSend(humidity, DHT22_humid_last, DHT22_humid_max_variation))
  {
    send = true;
    DHT22_humid_last = humidity;
  }

  if (send)
  {
    DHT22_sendReadingToMQTT(temperature, humidity, heatIndex);
  }
}
bool DHT22_ready = false;

void DHT22_Loop()
{
  
  while (!DHT22_ready)
  {
    Log.trace("Waiting for the DHT22 sensor to come online");
    float temperature = dht22.readTemperature();
    DHT22_ready = !isnan(temperature);
    Log.trace("Current reading:[%F]", temperature);
    if(DHT22_ready) {
      Log.trace("DHT22 sensor is online, proceeding: %F", temperature);
    } else {
      WaitingOnSensorLed();
      delay(100);
    }
  }
  checkAndReportReadings();
}

#endif
#endif