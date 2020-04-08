//AthosHelpers.h
#ifndef ATH_HELPERS_h
#define ATH_HELPERS_h

#include "AthosLED.h"
#include "Arduino.h"

class AthosHelpers {
    public:
        AthosHelpers();
        AthosHelpers(AthosLED led);
        String getVersion(void);
        String getBuild(void);
        String getDeviceId(void);
        void BlinkLed(void);
        void BlinkLed(int blinkCount, int blinkDelay);
        void MQTTTransmitLed(void);
        void WifiSetupStartLed(void);
        void WifiSetupCompleteLed(void);
        void Relay_ON_Led(void);
        void Relay_OFF_Led(void);
        void MotionDetected_LED(void);
        void NoMotionDetected_LED(void);
        bool shouldSend(float current, float last, float max_diff);
        
    private:
      time_t build_time(void);
      AthosLED _helperled;
};



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
  String mqttPingTopic;
  String mqttMotionTopic;
};
typedef struct storageValues StorageValues;


#endif
