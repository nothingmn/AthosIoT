//AthosRelay.h
#ifdef ATH_RELAY
#include "AthosEEPROM.h"
#include "AthosRelay.h"
#include "AthosNTP.h"
#include "AthosMQTT.h"
#include "AthosHelpers.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <ArduinoLog.h>

AthosNTP _relay_ntp;
AthosHelpers _relay_helpers;

//add or remove and sort items from the array to control the pins as you like.
uint RELAY_PINS[]{D0};
//change this so we can report up to to the server
String RELAY_Report_PINS = "D0";

#define turn_On 1
#define turn_Off 0

PubSubClient _Relay_mqtt_client;
String _Relay_deviceId;
StorageValues _Relay_config;
int _Relay_loop_delay;

void AthosRelay::Relay_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _Relay_mqtt_client = mqtt_client;
  _Relay_deviceId = deviceId;
  _Relay_config = rootConfig;
  _Relay_loop_delay = loop_delay;

  // Pin for relay module set as output
  for (int x = 0; x < sizeof(RELAY_PINS); x++)
  {
    pinMode(RELAY_PINS[x], OUTPUT);
    digitalWrite(RELAY_PINS[x], turn_Off);
  }
}

void AthosRelay::RunFun(void)
{
  for (int x = 0; x <= 5; x++)
  {
    for (int y = 0; y < sizeof(RELAY_PINS); y++)
    {
      digitalWrite(RELAY_PINS[y], turn_Off);
    }
    delay(100);
    for (int z = 0; z < sizeof(RELAY_PINS); z++)
    {
      digitalWrite(RELAY_PINS[z], turn_On);
      delay(100);
      digitalWrite(RELAY_PINS[z], turn_Off);
      delay(100);
    }
  }
}
bool AthosRelay::Relay_MQTT_Received(String topic, String json)
{

  if (topic.endsWith("/" + _Relay_deviceId))
  {

    Log.trace("Relay_MQTT_Received  \nTopic:%s\nPayload:%s", topic.c_str(), json.c_str());

    //matched to the current device.  now do something with the JSON payload;
    StaticJsonDocument<255> readDoc;
    deserializeJson(readDoc, json);
    String command = readDoc["command"].as<String>();

    if (command == "on" || command == "off")
    {
      int vpin = readDoc["pin"].as<int>();
      int pin = RELAY_PINS[0];

      Log.trace("Relay_MQTT_Received:\npin:%i\npin:%i\ncommand:%s", vpin, pin, command.c_str());

      if (vpin <= sizeof(RELAY_PINS))
      {
        pin = RELAY_PINS[vpin];
      }

      if (command == "on")
      {
        digitalWrite(pin, turn_On);
        return true;
      }
      else
      {
        digitalWrite(pin, turn_Off);
        return true;
      }
    }
    if (command == "allon")
    {
      for (int x = 0; x < sizeof(RELAY_PINS); x++)
      {
        digitalWrite(RELAY_PINS[x], turn_On);
      }
      return true;
    }
    if (command == "alloff")
    {
      for (int x = 0; x < sizeof(RELAY_PINS); x++)
      {
        digitalWrite(RELAY_PINS[x], turn_Off);
      }
      return true;
    }
    if (command == "fun")
    {
      RunFun();
      return true;
    }
  }
  return false;
}

void AthosRelay::RELAY_CheckIn(void)
{
  Log.trace("RELAY_CheckIn");
  int ts = _relay_ntp.NTP_getEpochTime();

  String csv = String("RELAY," + _relay_helpers.getVersion() + "," + ts + "," + RELAY_Report_PINS + "," + _Relay_deviceId);
  const char *payload = csv.c_str();
  const char *topic = _Relay_config.mqttSensorTopic.c_str();
  Log.trace("Topic:%s\nPayload:%s\nLength:%i\n", topic, payload, csv.length());

  if (!_Relay_mqtt_client.publish(topic, payload))
  {
    Log.trace("RELAY Checkin Data to MQTT Failed. Packet > 128?");
  }
  _relay_helpers.MQTTTransmitLed();
}

long relay_last = 0;
long relay_max_diff = 60;
void AthosRelay::Relay_Loop(void)
{
  long now = _relay_ntp.NTP_getEpochTime();
  long diff = abs(now - relay_last);
  if (diff > relay_max_diff)
  {
    relay_last = now;
    RELAY_CheckIn();
  }
}
/*
  Constructor
*/

AthosRelay::AthosRelay()
{
}

AthosRelay::AthosRelay(AthosHelpers helpers, AthosNTP ntp)
{
  _relay_ntp = ntp;
  _relay_helpers = helpers;
}

#endif
