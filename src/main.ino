#define ATH_WIFIMANAGER
#define ATH_NTP
//#define ATH_TMP36
#define ATH_DHT11
#define ATH_UDP
#define ATH_MQTT

#include <ESP8266WiFi.h>
#include "AthosHelpers.h"
#include "AthosEEPROM.h"
#include "AthosWifiManager.h"
#include "AthosNTP.h"
#include "AthosUDP.h"
#include "AthosMQTT.h"

#ifdef ATH_TMP36
  #include "AthosTMP36.h"
#endif

#ifdef ATH_DHT11
  #include "AthosDHT11.h"
#endif

String DeviceId = getDeviceId();
//our main loop delay.
int loop_delay = 1000;

StorageValues rootConfig;
PubSubClient root_mqtt_client;
void setup()
{

  Serial.begin(115200);
  delay(100);

  rootConfig = EEPROM_setup();

  #ifdef ATH_WIFIMANAGER
    Serial.println("WifiManager Start");      
    rootConfig = WifiManager_Setup(DeviceId, rootConfig);
    Serial.println("WifiManager Done");
  #endif
  
  #ifdef ATH_NTP
    Serial.println("NTP Start");      
    NTP_Setup();
    Serial.println("NTP Done");
  #endif


  #ifdef ATH_UDP
    Serial.println("UDP Start");      
    rootConfig = UDP_Setup(DeviceId, rootConfig);
    Serial.println("UDP Done");
  #endif


  #ifdef ATH_MQTT
    Serial.println("MQTT Start");      
    root_mqtt_client = MQTT_Setup(DeviceId, rootConfig);
    Serial.println("MQTT Done");
  #endif

  
  #ifdef ATH_TMP36
    Serial.println("TMP Start");      
    TMP_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
    Serial.println("TMP Done");      
  #endif

  
  #ifdef ATH_DHT11
    Serial.println("DHT11 Start");      
    DHT11_Setup(root_mqtt_client, DeviceId, rootConfig, loop_delay);
    Serial.println("DHT11 Done");      
  #endif

}

void loop()
{

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


  delay(loop_delay);
}
