//
//
//Step 1, Update the #define and #includes below for the types of sensors or the Relay features
//Step 2, deploy to your NodeMCU device
//
//
#define ATH_LED
#define ATH_WIFIMANAGER
#define ATH_NTP
#define ATH_UDP
#define ATH_MQTT

//#define ATH_RELAY
//#define ATH_TMP36
//#define ATH_DHT11
//#define ATH_BMP280

//disable logging
//#define DISABLE_LOGGING 


//setup which sensors, etc you want included.
#include <ArduinoLog.h>
#include <ESP8266WiFi.h>
#include "AthosLED.h"
#include "AthosHelpers.h"
#include "AthosEEPROM.h"
#include "AthosWifiManager.h"
#include "AthosNTP.h"
#include "AthosUDP.h"
#include "AthosRelay.h"
#include "AthosMQTT.h"
#include "AthosTMP36.h"
#include "AthosDHT11.h"
#include "AthosBMP280.h"


String DeviceId = getDeviceId();
//our main loop delay.
int loop_delay = 1000;

StorageValues rootConfig;
PubSubClient root_mqtt_client;
void setup()
{
  //while (!Serial);
  Serial.begin(115200);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  Log.setPrefix(printTimestamp); // Uncomment to get timestamps as prefix
  Log.setSuffix(printNewline); // Uncomment to get newline as suffix

  Log.trace("~~~~~~~~~~SETUP STARTING~~~~~~~~~~");

  rootConfig = EEPROM_setup();

#ifdef ATH_LED
  Log.trace("LED Start");
  LED_Setup();
  Log.trace("LED Done");
#endif

#ifdef ATH_WIFIMANAGER
  Log.trace("WifiManager Start");
  rootConfig = WifiManager_Setup(DeviceId, rootConfig);
  Log.trace("WifiManager Done");
#endif

#ifdef ATH_NTP
  Log.trace("NTP Start");
  NTP_Setup();
  Log.trace("NTP Done");
#endif

#ifdef ATH_UDP
  Log.trace("UDP Start");
  rootConfig = UDP_Setup(DeviceId, rootConfig);
  Log.trace("UDP Done");
#endif

#ifdef ATH_MQTT
  Log.trace("MQTT Start");
  root_mqtt_client = MQTT_Setup(DeviceId, rootConfig);
  delay(1000);
  Log.trace("MQTT Done");
#endif

#ifdef ATH_TMP36
  Log.trace("TMP Start");
  TMP_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("TMP Done");
#endif

#ifdef ATH_DHT11
  Log.trace("DHT11 Start");
  DHT11_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("DHT11 Done");
#endif

#ifdef ATH_BMP280
  Log.trace("BMP280 Start");
  BMP280_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("BMP280 Done");
#endif

#ifdef ATH_RELAY
  Log.trace("RELAY Start");
  Relay_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("RELAY Done");
#endif

  Log.trace("~~~~~~~~~~SETUP COMPLETED~~~~~~~~~~");
}

void loop()
{

#ifdef ATH_LED
  LED_Loop();
#endif

#ifdef ATH_WIFIMANAGER
  WifiManager_Loop();
#endif

#ifdef ATH_NTP
  NTP_Loop();
#endif

#ifdef ATH_UDP
  UDP_Loop();
#endif

#ifdef ATH_MQTT
  MQTT_Loop();
#endif

#ifdef ATH_TMP36
  TMP_Loop();
#endif

#ifdef ATH_DHT11
  DHT11_Loop();
#endif

#ifdef ATH_BMP280
  BMP280_Loop();
#endif

#ifdef ATH_RELAY
  Relay_Loop();
#endif

  delay(loop_delay);
}

