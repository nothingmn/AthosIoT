//
//
//Step 1, Update the #define and #includes below for the types of sensors or the Relay features
//Step 2, deploy to your NodeMCU device
//
//

//#define ATH_PIR
//#define ATH_RELAY
//#define ATH_TMP36
//#define ATH_DHT11
//#define ATH_BMP280

//disable logging
//#define DISABLE_LOGGING 


//setup which sensors, etc you want included.
#include "AthosLED.h"
#include "AthosHelpers.h"
#include "AthosEEPROM.h"
#include "AthosWifiManager.h"
#include "AthosNTP.h"
#include "AthosHTTP.h"
#include "AthosUDP.h"
#include "DeviceData.h"
#include "AthosRelay.h"
#include "AthosMQTT.h"
#include "AthosTMP36.h"
#include "AthosDHT11.h"
#include "AthosBMP280.h"
#include "AthosPIR.h"
#include "AthosMoisture.h"
#include "AthosMQ2GasSensor.h"

#include "Arduino.h"
#include <ArduinoLog.h>
#include <ESP8266WiFi.h>

String DeviceId = getDeviceId();

//our main loop delay.
int loop_delay = 1000 / 10;   //10Hz

StorageValues rootConfig;
PubSubClient root_mqtt_client;
void setup()
{
  //while (!Serial);
  Serial.begin(115200);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  //Log.setPrefix(printTimestamp); // Uncomment to get timestamps as prefix
  Log.setSuffix(printNewline); // Uncomment to get newline as suffix

  Log.trace("~~~~~~~~~~SETUP STARTING~~~~~~~~~~Version:%s Build:%s", getVersion().c_str(), getBuild().c_str());

  rootConfig = EEPROM_setup();


  Log.trace("LED Start");
  LED_Setup();
  Log.trace("LED Done");

  Log.trace("WifiManager Start");
  rootConfig = WifiManager_Setup(DeviceId, rootConfig);
  Log.trace("WifiManager Done");

  Log.trace("NTP Start");
  NTP_Setup();
  Log.trace("NTP Done");

  if(WiFi.gatewayIP().toString() == "10.3.141.1") {
    Log.trace("HTTP Start");
    rootConfig = HTTP_Setup(DeviceId, rootConfig);
    Log.trace("HTTP Done");
  } else {
    Log.trace("UDP Start");
    rootConfig = UDP_Setup(DeviceId, rootConfig);
    Log.trace("UDP Done");
  }

  Log.trace("MQTT Start");
  root_mqtt_client = MQTT_Setup(DeviceId, rootConfig);
  delay(1000);
  Log.trace("MQTT Done");

  Log.trace("DeviceData Start");
  DeviceData_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("DeviceData Done");

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


#ifdef ATH_PIR
  Log.trace("PIR Start");
  PIR_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("PIR Done");
#endif

#ifdef ATH_MOIST
  Log.trace("MOIST Start");
  MOIST_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("MOIST Done");
#endif

#ifdef ATH_MQ2
  Log.trace("GAS Start");
  MQ2_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("GAS Done");
#endif

  Log.trace("~~~~~~~~~~SETUP COMPLETED ~~~~~~~~~~Version:%s Build:%s", getVersion().c_str(), getBuild().c_str());
}

void loop()
{

  LED_Loop();
  WifiManager_Loop();
  NTP_Loop();
  HTTP_Loop();
  MQTT_Loop();
  DeviceData_Loop();

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

#ifdef ATH_PIR
  PIR_Loop();
#endif

#ifdef ATH_MOIST
  MOIST_Loop();
#endif

#ifdef ATH_MQ2
  MQ2_Loop();
#endif

  delay(loop_delay);
}

