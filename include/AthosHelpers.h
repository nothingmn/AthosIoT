#include <string>
#include <ESP8266WiFi.h>

#define ATH_HELPERS

struct storageValues
{
  String ssid;
  String password;
  String mqttServer;
  String mqttUsername;
  String mqttPassword;
  String mqttPort;
  String mqttSensorTopic;
  String mqttRelayTopic;
  String mqttCapsTopic;
  String mqttPingTopic;
  String mqttMotionTopic;
};

typedef struct storageValues StorageValues;

String getDeviceId()
{
  //B4:E6:2D: 34:AD:0C
  String address = WiFi.macAddress();
  std::string s(address.c_str());
  s = s.substr(9, 8);
  String sAddress = s.c_str();
  sAddress.remove(2, 1);
  sAddress.remove(4, 1);
  return sAddress;
}

void BlinkLed(int blinkCount, int blinkDelay)
{
#ifdef ATH_LED
  LED_Blink(blinkCount, blinkDelay);
#endif
}
void BlinkLed()
{
#ifdef ATH_LED
  LED_Blink();
#endif
}

void MQTTTransmitLed()
{
#ifdef ATH_LED
  LED_Blink(5);
#endif
}
void WifiSetupStartLed()
{
#ifdef ATH_LED
  LED_Blink(2);
#endif
}
void WifiSetupCompleteLed()
{
#ifdef ATH_LED
  LED_Blink(3);
#endif
}

void Relay_ON_Led()
{
#ifdef ATH_LED
  LED_ON();
#endif
}
void Relay_OFF_Led()
{
#ifdef ATH_LED
  LED_OFF();
#endif
}

void MotionDetected_LED()
{
#ifdef ATH_LED
  LED_Blink(4);
#endif
}

void NoMotionDetected_LED()
{
#ifdef ATH_LED
  LED_Blink(3);
#endif
}




void printTimestamp(Print* _logOutput) {
  char c[12];
  sprintf(c, "%10lu ", millis());
  _logOutput->print(c);
}

void printNewline(Print* _logOutput) {
  _logOutput->print('\n');
}