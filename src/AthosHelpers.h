#include <string>

#ifndef ATH_HELPERS
  #define ATH_HELPERS
#endif

struct storageValues {
  String ssid;
  String password;
  String mqttServer;
  String mqttUsername;
  String mqttPassword;
  String mqttPort;
  String mqttSensorTopic;  
  String mqttRelayTopic;  
  String mqttCapsTopic;  
};
typedef struct storageValues StorageValues;


String getDeviceId() {
  //B4:E6:2D: 34:AD:0C
  String address = WiFi.macAddress();
  std::string s(address.c_str());
  s = s.substr(9,8);
  String sAddress = s.c_str();
  sAddress.remove(2,1);
  sAddress.remove(4,1);
  return sAddress;
}

void BlinkLed(int blinkCount, int blinkDelay) {
  #ifdef ATH_LED 
    LED_Blink(blinkCount, blinkDelay);
  #endif
}
void BlinkLed() {
  #ifdef ATH_LED 
    LED_Blink();
  #endif
}

void MQTTTransmitLed() {
  #ifdef ATH_LED 
    LED_Blink(5);
  #endif
}
void WifiSetupStartLed() {
  #ifdef ATH_LED 
    LED_Blink(2);
  #endif
}
void WifiSetupCompleteLed() {
  #ifdef ATH_LED 
    LED_Blink(3);
  #endif
}

void Relay_ON_Led() {
  #ifdef ATH_LED 
    LED_ON();
  #endif
}
void Relay_OFF_Led() {
  #ifdef ATH_LED 
    LED_OFF();
  #endif
}


