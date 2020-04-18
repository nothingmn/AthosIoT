#ifndef ATH_HELPERS
#define ATH_HELPERS


#include <string>
#include <ESP8266WiFi.h>
//#include <time.h>

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
  String mqttPingTopic;
  String mqttMotionTopic;
};

typedef struct storageValues StorageValues;


time_t build_time() {
  static const char *built = __DATE__ " " __TIME__;  
  struct tm t;
  strptime(built, "%b %d %Y %H:%M:%S", &t);
  return mktime(&t);
}


String  _version;
String getVersion(void)
{
  if (_version != NULL)
  {
    return _version;
  }
  char buff[25];  

  time_t now = build_time();
  size_t size = strftime(buff, 25, "1.0.%Y%m%d.%H.%M", localtime(&now));
  _version = String(buff);
  return _version;
}

String  _build;
String getBuild() {

  if (_build != NULL)
  {
    return _build;
  }

String build = "";

#ifdef ATH_PIR
  build = build + "PIR,";
#endif
#ifdef ATH_RELAY
  build = build + "RELAY,";
#endif
#ifdef ATH_TMP36
  build = build + "TMP36,";
#endif
#ifdef ATH_DHT11
  build = build + "DHT11,";
#endif
#ifdef ATH_BMP280
  build = build + "BMP280,";
#endif
  
  _build = String(build.substring(0, build.length()-1));
  return _build;
}

String getDeviceId()
{
  String address = WiFi.macAddress();
  std::string s(address.c_str());
  s = s.substr(9, 8);
  String sAddress = s.c_str();
  sAddress.remove(2, 1);
  sAddress.remove(4, 1);
  return sAddress;
}

void WarmingUpLed() {
  LED_Blink(10, 25);
}

void WarmedUpLed() {
  LED_Blink(3, 1000);
}

void WaitingOnSensorLed() {
  LED_Blink(5, 2000);
}


void BlinkLed(int blinkCount, int blinkDelay)
{
  LED_Blink(blinkCount, blinkDelay);
}
void BlinkLed()
{
  LED_Blink();
}

void MQTTTransmitLed()
{
  LED_Blink(5);
}
void WifiSetupStartLed()
{
  LED_Blink(2);
}
void WifiSetupCompleteLed()
{
  LED_Blink(3);
}

void Relay_ON_Led()
{
  LED_ON();
}
void Relay_OFF_Led()
{
  LED_OFF();
}

void MotionDetected_LED()
{
  LED_Blink(4);
}

void NoMotionDetected_LED()
{
  LED_Blink(3);
}

bool shouldSend(float current, float last, float max_diff)
{
  float diff = abs(current - last);
  return (diff > max_diff);
}

void printTimestamp(Print *_logOutput)
{
  char c[12];
  sprintf(c, "%10lu ", millis());
  _logOutput->print(c);
}

void printNewline(Print *_logOutput)
{
  _logOutput->print('\n');
}


#endif