//AthosRelay.h
#ifdef ATH_RELAY
#ifndef ATH_Relay_h
#define ATH_Relay_h

#include "AthosHelpers.h"
#include "AthosNTP.h"
#include <PubSubClient.h>

class AthosRelay {
    public:
        AthosRelay();
        AthosRelay(AthosHelpers helpers, AthosNTP ntp);
        void Relay_Loop(void);
        void Relay_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay);
        bool Relay_MQTT_Received(String topic, String json);

    private:
        void RELAY_CheckIn(void);
        void RunFun(void);
        AthosNTP _relay_ntp;
        AthosHelpers _relay_helpers;
        uint RELAY_PINS[];
        String RELAY_Report_PINS;
        PubSubClient _Relay_mqtt_client;
        String _Relay_deviceId;
        StorageValues _Relay_config;
        int _Relay_loop_delay;

};
#endif
#endif
