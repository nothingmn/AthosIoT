//AthosHelpers.cpp

#include "AthosHelpers.h"
#include "AthosLED.h"
#include <string>
#include <ESP8266WiFi.h>
#include <time.h>

AthosLED _helperled;

typedef struct storageValues StorageValues;


time_t AthosHelpers::build_time(void) {
  static const char *built = __DATE__ " " __TIME__;  
  struct tm t;
  strptime(built, "%b %d %Y %H:%M:%S", &t);
  return mktime(&t);
}


String  _version;
String AthosHelpers::getVersion(void)
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
String AthosHelpers::getBuild(void) {

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

String AthosHelpers::getDeviceId(void)
{
  String address = WiFi.macAddress();
  std::string s(address.c_str());
  s = s.substr(9, 8);
  String sAddress = s.c_str();
  sAddress.remove(2, 1);
  sAddress.remove(4, 1);
  return sAddress;
}

void AthosHelpers::BlinkLed(int blinkCount, int blinkDelay)
{
  _helperled.LED_Blink(blinkCount, blinkDelay);
}
void AthosHelpers::BlinkLed(void)
{
  _helperled.LED_Blink();
}

void AthosHelpers::MQTTTransmitLed(void)
{
  _helperled.LED_Blink(5);
}
void AthosHelpers::WifiSetupStartLed(void)
{
  _helperled.LED_Blink(2);
}
void AthosHelpers::WifiSetupCompleteLed(void)
{
  _helperled.LED_Blink(3);
}

void AthosHelpers::Relay_ON_Led(void)
{
  _helperled.LED_ON();
}
void AthosHelpers::Relay_OFF_Led(void)
{
  _helperled.LED_OFF();
}

void AthosHelpers::MotionDetected_LED(void)
{
  _helperled.LED_Blink(4);
}

void AthosHelpers::NoMotionDetected_LED(void)
{
  _helperled.LED_Blink(3);
}

bool AthosHelpers::shouldSend(float current, float last, float max_diff)
{
  float diff = abs(current - last);
  return (diff > max_diff);
}


/*
  Constructor
*/
AthosHelpers::AthosHelpers()
{
  _helperled = AthosLED();
}

AthosHelpers::AthosHelpers(AthosLED led)
{
  _helperled = led;
}
