//
//
//Step 1, Update the #define and #includes below for the types of sensors or the Relay features
//Step 2, deploy to your NodeMCU device
//
//

//disable logging
//#define DISABLE_LOGGING 

#include <ArduinoLog.h>
#include <ESP8266WiFi.h>

//setup which sensors, etc you want included.
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
#include "AthosPIR.h"


AthosLED _led = AthosLED();
AthosHelpers _helpers = AthosHelpers(_led);
AthosEEPROM _eeprom = AthosEEPROM();
AthosNTP _ntp = AthosNTP();
AthosWifiManager _wifi = AthosWifiManager(_helpers, _eeprom);
AthosUDP _udp = AthosUDP(_helpers, _ntp, _eeprom);
AthosMQTT _mqtt = AthosMQTT(_helpers, _ntp, _eeprom);

#ifdef ATH_TMP36
  AthosTMP36 _tmp36 = AthosTMP36(_helpers, _ntp);
#endif

#ifdef ATH_DHT11
  AthosDHT11 _dht11 = AthosDHT11(_helpers, _ntp);
#endif

#ifdef ATH_BMP280
  AthosBMP280 _bmp280 = AthosBMP280(_helpers, _ntp);
#endif

#ifdef ATH_RELAY
  AthosRelay _relay = AthosRelay(_helpers, _ntp);
#endif

#ifdef ATH_PIR
  AthosPIR _pir = AthosPIR(_helpers, _ntp);
#endif


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
String DeviceId = _helpers.getDeviceId();

//our main loop delay.
int loop_delay = 1000 / 10;   //10Hz

StorageValues rootConfig;
PubSubClient root_mqtt_client;
void setup()
{
  //while (!Serial);
  Serial.begin(115200);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  //Log.setPrefix(_helpers.printTimestamp); // Uncomment to get timestamps as prefix
  Log.setSuffix(printNewline); // Uncomment to get newline as suffix

  Log.trace("~~~~~~~~~~SETUP STARTING~~~~~~~~~~Version:%s Build:%s", _helpers.getVersion().c_str(), _helpers.getBuild().c_str());

  rootConfig = _eeprom.EEPROM_setup();

  Log.trace("LED Start");
  _led.LED_Setup();
  Log.trace("LED Done");

#ifdef ATH_WIFIMANAGER
  Log.trace("WifiManager Start");
  rootConfig = _wifi.WifiManager_Setup(DeviceId, rootConfig);
  Log.trace("WifiManager Done");
#endif

#ifdef ATH_NTP
  Log.trace("NTP Start");
  _ntp.NTP_Setup();
  Log.trace("NTP Done");
#endif

#ifdef ATH_UDP
  Log.trace("UDP Start");
  rootConfig = _udp.UDP_Setup(DeviceId, rootConfig);
  Log.trace("UDP Done");
#endif

#ifdef ATH_MQTT
  Log.trace("MQTT Start");
  root_mqtt_client = _mqtt.MQTT_Setup(DeviceId, rootConfig);
  delay(1000);
  Log.trace("MQTT Done");
#endif

#ifdef ATH_TMP36
  Log.trace("TMP Start");
  _tmp36.TMP_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("TMP Done");
#endif

#ifdef ATH_DHT11
  Log.trace("DHT11 Start");
  _dht11.DHT11_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("DHT11 Done");
#endif

#ifdef ATH_BMP280
  Log.trace("BMP280 Start");
  _bmp280.BMP280_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("BMP280 Done");
#endif

#ifdef ATH_RELAY
  Log.trace("RELAY Start");
  _relay.Relay_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("RELAY Done");
#endif


#ifdef ATH_PIR
  Log.trace("PIR Start");
  _pir.PIR_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
  Log.trace("PIR Done");
#endif


  Log.trace("~~~~~~~~~~SETUP COMPLETED ~~~~~~~~~~Version:%s Build:%s", _helpers.getVersion().c_str(),  _helpers.getBuild().c_str());
}

void loop()
{

  _led.LED_Loop();

#ifdef ATH_WIFIMANAGER
  _wifi.WifiManager_Loop();
#endif

#ifdef ATH_NTP
  _ntp.NTP_Loop();
#endif

#ifdef ATH_UDP
  _udp.UDP_Loop();
#endif

#ifdef ATH_MQTT
  _mqtt.MQTT_Loop();
#endif

#ifdef ATH_TMP36
  _tmp36.TMP_Loop();
#endif

#ifdef ATH_DHT11
  _dht11.DHT11_Loop();
#endif

#ifdef ATH_BMP280
  _bmp280.BMP280_Loop();
#endif

#ifdef ATH_RELAY
  _relay.Relay_Loop();
#endif

#ifdef ATH_PIR
  _pir.PIR_Loop();
#endif

  delay(loop_delay);
}

