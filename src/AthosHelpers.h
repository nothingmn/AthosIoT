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
