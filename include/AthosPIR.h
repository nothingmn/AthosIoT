///Make sure you have a 3V PIR Sensor
///In this sketch, we are using a D0 for the data pin

#ifdef ATH_PIR
#include <ArduinoJson.h>
#include <PubSubClient.h>

PubSubClient _PIR_mqtt_client;
String _PIR_deviceId;
StorageValues _PIR_config;
int _PIR_loop_delay;

int PIR_inputPin = D0; // choose the input pin (for PIR sensor)
int PIR_state = LOW;  // we start, assuming no motion detected
int PIR_val = 0;      // variable for reading the pin status

void PIR_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _PIR_mqtt_client = mqtt_client;
  _PIR_deviceId = deviceId;
  _PIR_config = rootConfig;
  _PIR_loop_delay = loop_delay;

  pinMode(PIR_inputPin, INPUT);
  //digitalWrite(PIR_inputPin, PIR_state);
}


void sendMovementToMQTT(int movement)
{
  long ts = NTP_getEpochTime();

  StaticJsonDocument<200> doc;
  doc["move"] = movement;
  doc["ts"] = ts;
  doc["deviceid"] = _PIR_deviceId;
  String json;
  serializeJson(doc, json);
  Log.trace("%s %s", _PIR_config.mqttMotionTopic.c_str(), json.c_str());
  
  if (!_PIR_mqtt_client.publish(_PIR_config.mqttMotionTopic.c_str(), json.c_str())) {
      Log.trace(F("Failed"));
  }

  MQTTTransmitLed();
}


void PIR_Loop()
{
  PIR_val = digitalRead(PIR_inputPin); // read input value
  if (PIR_val == HIGH) // check if the input is HIGH
  {

    if (PIR_state == LOW)
    {
      Log.trace("Motion detected!"); // print on output change
      MotionDetected_LED();
      PIR_state = HIGH;
      sendMovementToMQTT(PIR_val);
    }
  }
  else
  {
    if (PIR_state == HIGH)
    {
      Log.trace("Motion ended!"); // print on output change
      NoMotionDetected_LED();
      PIR_state = LOW;
      sendMovementToMQTT(PIR_val);
    }
  }
}

#endif
