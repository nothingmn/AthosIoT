//AthosWifiManager.h

#ifndef ATH_WifiManager_h
#define ATH_WifiManager_h

#include "AthosHelpers.h"
#include "AthosEEPROM.h"
#include <ESP8266WebServer.h>

class AthosWifiManager {
    public:
        AthosWifiManager();
        AthosWifiManager(AthosHelpers helpers, AthosEEPROM eeprom);
        StorageValues WifiManager_Setup(String deviceId, StorageValues rootConfig);
        void WifiManager_Loop();

    private:
        bool testWifi(void);
        void createWebServer(void);
        void launchWeb(void);
        void setupAP(void);
        void EnsureWifiConnected(void);
        AthosEEPROM _wifi_eeprom;
        AthosHelpers _wifi_helpers;
        String _deviceId;
        StorageValues _wifi_config;
        String _wifiAPName;
        String WIFI_st;
        int WIFI_statusCode;

};
#endif
