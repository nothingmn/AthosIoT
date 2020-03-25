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

//Configure your logging level.  If you define VERBOSE, it will enable all levels
//otherwise just enable DEBUG/INFO/ERROR as you see fit.
#define ATH_LOG_VERBOSE
//#define ATH_LOG_DEBUG
//#define ATH_LOG_INFO
//#define ATH_LOG_ERROR

//setup which sensors, etc you want included.
#include <ESP8266WiFi.h>
#include "AthosLog.h"
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

  log_info("~~~~~~~~~~SETUP STARTING~~~~~~~~~~");

  rootConfig = EEPROM_setup();

#ifdef ATH_LED
  log_info("LED Start");
  LED_Setup();
  log_info("LED Done");
#endif

#ifdef ATH_WIFIMANAGER
  log_info("WifiManager Start");
  rootConfig = WifiManager_Setup(DeviceId, rootConfig);
  log_info("WifiManager Done");
#endif

#ifdef ATH_NTP
  log_info("NTP Start");
  NTP_Setup();
  log_info("NTP Done");
#endif

#ifdef ATH_UDP
  log_info("UDP Start");
  rootConfig = UDP_Setup(DeviceId, rootConfig);
  log_info("UDP Done");
#endif

#ifdef ATH_MQTT
  log_info("MQTT Start");
  root_mqtt_client = MQTT_Setup(DeviceId, rootConfig);
  delay(1000);
  log_info("MQTT Done");
#endif

#ifdef ATH_TMP36
  log_info("TMP Start");
  TMP_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  log_info("TMP Done");
#endif

#ifdef ATH_DHT11
  log_info("DHT11 Start");
  DHT11_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  log_info("DHT11 Done");
#endif

#ifdef ATH_BMP280
  log_info("BMP280 Start");
  BMP280_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  log_info("BMP280 Done");
#endif

#ifdef ATH_RELAY
  log_info("RELAY Start");
  Relay_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  log_info("RELAY Done");
#endif

  log_info("~~~~~~~~~~SETUP COMPLETED~~~~~~~~~~");
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
