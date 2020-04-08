//AthosPIR.h

#ifndef ATH_PIR_h
#define ATH_PIR_h

#include "AthosHelpers.h"
#include "AthosNTP.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>

class AthosPIR {
    public:
        AthosPIR();
        AthosPIR(AthosHelpers helpers, AthosNTP ntp);
        void PIR_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay);
        void PIR_Loop(void);
        
    private:
        void sendMovementToMQTT(int movement);
        PubSubClient _PIR_mqtt_client;
        String _PIR_deviceId;
        StorageValues _PIR_config;
        int _PIR_loop_delay;
        int PIR_inputPin;
        int PIR_state;
        int PIR_val;
        AthosHelpers _pir_helpers;
        AthosNTP _pir_ntp;
};
#endif
