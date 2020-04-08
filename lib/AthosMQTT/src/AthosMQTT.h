//AthosMQTT.h

#ifndef ATH_MQTT_h
#define ATH_MQTT_h
#include "AthosEEPROM.h"
#include "AthosHelpers.h"
#include "AthosRelay.h"
#include "AthosNTP.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>



class AthosMQTT {
    public:
        AthosMQTT(void);
        AthosMQTT(AthosHelpers helpers, AthosNTP ntp, AthosEEPROM eeprom);
        PubSubClient MQTT_Setup(String deviceId, StorageValues rootConfig);
        void MQTT_Loop(void);
        void MQTT_Callback(char *topic, uint8_t *payload, unsigned int length);
        
#ifdef ATH_RELAY
        void InitRelay(AthosRelay relay);
#endif
        
    private:
        bool ConnectToMqtt(void);
        void MQTT_PongResponse(int senderTS);        
        void ConnectAndSubscribe(void);
        String GetValueOrDefault(StaticJsonDocument<256> doc, String group, String name, String defaultValue);
        AthosHelpers _mqtt_helpers;
        AthosNTP _mqtt_ntp;
        AthosEEPROM _eeprom;
        PubSubClient mqtt_client;
        String _mqtt_deviceId;
        StorageValues _mqtt_config;
#ifdef ATH_RELAY
        AthosRelay _mqtt_relay;
#endif

};
#endif
