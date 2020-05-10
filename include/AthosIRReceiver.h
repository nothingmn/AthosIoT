#ifdef ATH_IRRECEIVER
#ifndef ATH_IRRECEIVER_
#define ATH_IRRECEIVER_

///Make sure you have a 3V or 5V IRRECEIVER Sensor
///In this sketch, we are using a D1 for the data pin
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include "Arduino.h"
#include <PubSubClient.h>

PubSubClient _IRRECEIVER_mqtt_client;
String _IRRECEIVER_deviceId;
StorageValues _IRRECEIVER_config;
int _IRRECEIVER_loop_delay;


const uint16_t IRRECEIVER_inputPin = D5; // choose the input pin (for IRRECEIVER sensor)
int IRRECEIVER_val = 0;      // variable for reading the pin status
IRrecv IRRECEIVER_irrecv(IRRECEIVER_inputPin);
decode_results IRRECEIVER_results;

void IRRECEIVER_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _IRRECEIVER_mqtt_client = mqtt_client;
  _IRRECEIVER_deviceId = deviceId;
  _IRRECEIVER_config = rootConfig;
  _IRRECEIVER_loop_delay = loop_delay;

  IRRECEIVER_irrecv.enableIRIn();  // Start the receiver

}


void sendIRMQTT(decode_results results)
{
  long ts = NTP_getEpochTime();

  String csv = String("IR," + getVersion() + "," + ts + "," + results.value + "," + _IRRECEIVER_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _IRRECEIVER_config.mqttSensorTopic.c_str();
  Log.trace("Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!_IRRECEIVER_mqtt_client.publish(topic, payload)) {
    Log.trace("IRRECEIVER Data to MQTT Failed. Packet > 128?");
  }

  MQTTTransmitLed();
}


void IRRECEIVER_Loop()
{
  
  if (IRRECEIVER_irrecv.decode(&IRRECEIVER_results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
    serialPrintUint64(IRRECEIVER_results.value, HEX);
    irrecv.resume();  // Receive the next value

  }
  
}

#endif
#endif