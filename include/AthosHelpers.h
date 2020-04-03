#include <string>
#include <ESP8266WiFi.h>
//#include <time.h>

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


time_t build_time() {
  static const char *built = __DATE__ " " __TIME__;  
  struct tm t;
  strptime(built, "%b %d %Y %H:%M:%S", &t);
  return mktime(&t);
}


String _version;
String getVersion()
{
  if (_version != NULL)
  {
    return _version;
  }
  char buff[20];  
  time_t now = build_time();
  strftime(buff, 20, "1.0.%Y%m%d.%H%M%S", localtime(&now));
  _version = String(buff);
  return _version;
}

String getBuild() {
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

  return build.substring(0, build.length()-1);
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
