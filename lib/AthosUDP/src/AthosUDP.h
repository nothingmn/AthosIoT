//AthosUDP.h

#ifndef ATH_UDP_h
#define ATH_UDP_h

#include "AthosHelpers.h"
#include "AthosNTP.h"
#include "AthosEEPROM.h"
#include <ESP8266WiFi.h>

class AthosUDP {
    public:
        AthosUDP();
        AthosUDP(AthosHelpers helpers, AthosNTP ntp, AthosEEPROM eeprom);
        StorageValues UDP_Setup(String DeviceId, StorageValues rootConfig);
        void UDP_Loop(void);

    private:
        void listenForUDPMessages(void);
        void publishUDP(void);
        AthosHelpers _udp_helpers;
        AthosNTP _udp_ntp;        
        AthosEEPROM _udp_eeprom;
        String udp_deviceId;
        StorageValues _udp_config;
        bool UDP_configComplete;
        long last_udp_broadcast;


};
#endif
