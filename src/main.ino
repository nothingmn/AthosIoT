//
//
//Step 1, Update the #define and #includes below for the types of sensors or the Relay features
//Step 2, deploy to your NodeMCU device
//
//

String DEFAULT_WIFI_SSID = "IoT";
String  DEFAULT_WIFI_PASSWORD = "88888888";

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
#include "AthosDeviceData.h"
#include "AthosRelay.h"
#include "AthosMQTT.h"
#include "AthosTMP36.h"
#include "AthosDHT11.h"
#include "AthosDHT22.h"
#include "AthosBMP280.h"
#include "AthosPIR.h"
#include "AthosButton.h"
#include "AthosMoisture.h"
#include "AthosMQ2GasSensor.h"
#include "AthosMQ135GasSensor.h"
#include "AthosMQ9GasSensor.h"
#include "AthosMQ8GasSensor.h"
#include "AthosMQ7GasSensor.h"
#include "AthosMQ6GasSensor.h"
#include "AthosMQ5GasSensor.h"
#include "AthosMQ4GasSensor.h"
#include "AthosMQ3GasSensor.h"
#include "AthosIRReceiver.h"
#include "AthosNeoPixel.h"
#include "Arduino.h"
#include <ArduinoLog.h>
#include <ESP8266WiFi.h>




String DeviceId = getDeviceId();

//our main loop delay.
#ifdef ATH_IRRECEIVER
  int loop_delay = 50; 
#else
  int loop_delay = 1000 / 10;   //10Hz
#endif 

void dumpResetReason() {

  Serial.println("------Dumping last reset reason------");

  struct rst_info *rtc_info = system_get_rst_info();

  Serial.printf("reset reason: %x\n", rtc_info->reason);

  if (rtc_info->reason == REASON_WDT_RST || rtc_info->reason == REASON_EXCEPTION_RST || rtc_info->reason == REASON_SOFT_WDT_RST)
  {
      if (rtc_info->reason == REASON_EXCEPTION_RST)
      {
          Serial.printf("Fatal exception (%d):\n", rtc_info->exccause);
      }
      //The address of the last crash is printed, which is used to debug garbled output.
      Serial.printf("epc1=0x%08x, epc2=0x%08x, epc3=0x%08x, excvaddr=0x%08x, depc=0x%08x\n", rtc_info->epc1, rtc_info->epc2, rtc_info->epc3, rtc_info->excvaddr, rtc_info->depc);
  }
  if(rtc_info->reason == 0) Serial.println("0 = Power reboot");
  if(rtc_info->reason == 1) Serial.println("1 = Hardware WDT reset");
  if(rtc_info->reason == 2) Serial.println("2 = Fatal exception");
  if(rtc_info->reason == 3) Serial.println("3 = Software watchdog reset");
  if(rtc_info->reason == 4) Serial.println("4 = Software reset");
  if(rtc_info->reason == 5) Serial.println("5 = Deep-sleep");
  if(rtc_info->reason == 6) Serial.println("6 = Hardware reset");
  Serial.println("------END Dumping last reset reason------");
}

StorageValues rootConfig;
PubSubClient root_mqtt_client;
void setup()
{
  //while (!Serial);
  Serial.begin(115200);
  while (!Serial){  // Wait for the serial connection to be establised.
    delay(50);
  }
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  delay(5000);
  //Log.setPrefix(printTimestamp); // Uncomment to get timestamps as prefix
  Log.setSuffix(printNewline); // Uncomment to get newline as suffix

  Log.trace("~~~~~~~~~~SETUP STARTING~~~~~~~~~~Version:%s Build:%s", getVersion().c_str(), getBuild().c_str());

  dumpResetReason();

  rootConfig = EEPROM_setup();

  if(rootConfig.ssid == "" || rootConfig.ssid == "null") {
    rootConfig.ssid = DEFAULT_WIFI_SSID;
    rootConfig.password = DEFAULT_WIFI_PASSWORD;
    Log.trace("using default SSID and password for wifi: %s", DEFAULT_WIFI_SSID.c_str());
  }


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

#ifdef ATH_DHT22
  Log.trace("DHT22 Start");
  DHT22_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("DHT22 Done");
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

#ifdef ATH_BUTTON
  Log.trace("Button Start");
  BUTTON_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("Button Done");
#endif

#ifdef ATH_MOIST
  Log.trace("MOIST Start");
  MOIST_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("MOIST Done");
#endif

#ifdef ATH_MQ2
  Log.trace("MQ2 GAS Start");
  MQ2_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("MQ2 GAS Done");
#endif


#ifdef ATH_MQ135
  Log.trace("MQ135 GAS Start");
  MQ135_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("MQ135 GAS Done");
#endif


#ifdef ATH_MQ9
  Log.trace("MQ9 GAS Start");
  MQ9_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("MQ9 GAS Done");
#endif

#ifdef ATH_MQ8
  Log.trace("MQ8 GAS Start");
  MQ8_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("MQ8 GAS Done");
#endif

#ifdef ATH_MQ7
  Log.trace("MQ7 GAS Start");
  MQ7_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("MQ7 GAS Done");
#endif

#ifdef ATH_MQ6
  Log.trace("MQ6 GAS Start");
  MQ6_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("MQ6 GAS Done");
#endif

#ifdef ATH_MQ5
  Log.trace("MQ5 GAS Start");
  MQ5_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("MQ5 GAS Done");
#endif

#ifdef ATH_MQ4
  Log.trace("MQ4 GAS Start");
  MQ4_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("MQ4 GAS Done");
#endif

#ifdef ATH_MQ3
  Log.trace("MQ3 GAS Start");
  MQ3_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("MQ3 GAS Done");
#endif

#ifdef ATH_NEOPIXEL
  Log.trace("NeoPixel Start");
  NeoPixel_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("NeoPixel Done");
#endif

#ifdef ATH_IRRECEIVER
  Log.trace("IRRECEIVER Start");
  IRRECEIVER_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("IRRECEIVER Done");
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

#ifdef ATH_DHT22
  DHT22_Loop();
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

#ifdef ATH_BUTTON
  BUTTON_Loop();
#endif

#ifdef ATH_MOIST
  MOIST_Loop();
#endif

#ifdef ATH_MQ2
  MQ2_Loop();
#endif

#ifdef ATH_MQ135
  MQ135_Loop();
#endif

#ifdef ATH_MQ9
  MQ9_Loop();
#endif

#ifdef ATH_MQ8
  MQ8_Loop();
#endif

#ifdef ATH_MQ7
  MQ7_Loop();
#endif

#ifdef ATH_MQ6
  MQ6_Loop();
#endif

#ifdef ATH_MQ5
  MQ5_Loop();
#endif

#ifdef ATH_MQ4
  MQ4_Loop();
#endif

#ifdef ATH_MQ3
  MQ3_Loop();
#endif


#ifdef ATH_NEOPIXEL
  NeoPixel_Loop();
#endif

#ifdef ATH_IRRECEIVER
  IRRECEIVER_Loop();
#endif

  delay(loop_delay);
}

