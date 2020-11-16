#ifdef ATH_IRRECEIVER
#ifndef ATH_IRRECEIVER_
#define ATH_IRRECEIVER_

///Make sure you have a 3V or 5V IRRECEIVER Sensor
///In this sketch, we are using a D5 (Pin 14) for the data pin
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include "Arduino.h"
#include <PubSubClient.h>

PubSubClient _IRRECEIVER_mqtt_client;
String _IRRECEIVER_deviceId;
StorageValues _IRRECEIVER_config;
int _IRRECEIVER_loop_delay;

int IRRECEIVER_inputPin = D5;
const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 50; // Milli-Seconds
int IRRECEIVER_val = 0;      // variable for reading the pin status
IRrecv IRRECEIVER_irrecv(IRRECEIVER_inputPin, kCaptureBufferSize, kTimeout, true);
decode_results IRRECEIVER_results;
const char *IRRECEIVER_IgnoreHex = "FFFFFF";

void IRRECEIVER_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _IRRECEIVER_mqtt_client = mqtt_client;
  _IRRECEIVER_deviceId = deviceId;
  _IRRECEIVER_config = rootConfig;
  _IRRECEIVER_loop_delay = loop_delay;

  // Ignore messages with less than minimum on or off pulses.
  const uint16_t kMinUnknownSize = 12;
  const uint8_t kTolerancePercentage = kTolerance; // kTolerance is normally 25%
  //IRRECEIVER_irrecv.setUnknownThreshold(kMinUnknownSize);
  IRRECEIVER_irrecv.setTolerance(kTolerancePercentage); // Override the default tolerance.
  IRRECEIVER_irrecv.enableIRIn();                       // Start the receiver
}

void sendIRMQTT(String hex)
{
  long ts = NTP_getEpochTime();

  String csv = String("IR," + getVersion() + "," + ts + "," + hex + "," + _IRRECEIVER_deviceId);
  const char *payload = csv.c_str();
  const char *topic = _IRRECEIVER_config.mqttSensorTopic.c_str();
  Log.trace("Topic:%s\nPayload:%s\nLength:%i\n", topic, payload, csv.length());

  if (!_IRRECEIVER_mqtt_client.publish(topic, payload))
  {
    Log.trace("IRRECEIVER Data to MQTT Failed. Packet > 128?");
  }

  MQTTTransmitLed();
}

void IRRECEIVER_Loop()
{

  if (IRRECEIVER_irrecv.decode(&IRRECEIVER_results))
  {
    String hex = uint64ToString(IRRECEIVER_results.value, HEX);
    String human = resultToHumanReadableBasic(&IRRECEIVER_results);
    // Check if we got an IR message that was to big for our capture buffer.
    Log.trace("human:%s", human.c_str());
#ifdef ATH_RELAY
      Relay_IR_Received(hex);
#endif

#ifdef ATH_NEOPIXEL
      NeoPixel_IR_Received(hex);
#endif

      sendIRMQTT(hex);
    //IRRECEIVER_irrecv.resume();  // Receive the next value
  }
}

#endif
#endif