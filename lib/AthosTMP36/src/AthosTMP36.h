//AthosTMP36.h

#ifndef ATH_TMP36_h
#define ATH_TMP36_h

#include "AthosHelpers.h"
#include "AthosNTP.h"
#include "AnalogSmooth.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>

class AthosTMP36 {
    public:
        AthosTMP36();
        AthosTMP36(AthosHelpers helpers, AthosNTP ntp);
        void TMP_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay);
        void TMP_Loop(void);
        
    private:
        float TMP36_readTemperature(void);
        void sendTemperatureToMQTT(float value, float diff);
        void checkAndReportReadings(void);
        AthosHelpers _tmp36_helpers;
        AthosNTP _tmp36_ntp;
        float temp_max_variation;
        int TMP36_sensorPin;
        int _tmp36_loop_delay;
        PubSubClient _tmp36_mqtt_client;
        String _tmp36_deviceId;
        StorageValues _tmp36_config;
        AnalogSmooth TMP_AnalogSmooth;
};
#endif
