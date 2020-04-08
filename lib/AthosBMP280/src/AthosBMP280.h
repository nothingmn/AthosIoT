//AthosBMP280.h

#ifndef ATH_BMP280_h
#define ATH_BMP280_h

#include "AthosHelpers.h"
#include "AthosNTP.h"
#include "AnalogSmooth.h"
#include <PubSubClient.h>
#include <Adafruit_BME280.h>

class AthosBMP280 {
    public:
        AthosBMP280();
        AthosBMP280(AthosHelpers helpers, AthosNTP ntp);
        void BMP280_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay);
        void BMP280_Loop(void);
    private:
        void sendReadingToMQTT(float temp, float humidity, float pressure, float altitude);
        void BMP280_checkAndReportReadings(void);
        int _BMP280_loop_delay;
        Adafruit_BME280 bme;
        uint8_t i2cAddress;
        PubSubClient _BMP280_mqtt_client;
        String _BMP280_deviceId;
        StorageValues _BMP280_config;
        AnalogSmooth BMP_AnalogSmooth_temp;
        AnalogSmooth BMP_AnalogSmooth_humid;
        AnalogSmooth BMP_AnalogSmooth_press;
        AnalogSmooth BMP_AnalogSmooth_alt;
        AthosHelpers _bmp_helpers;
        AthosNTP _bmp_ntp;

};
#endif
