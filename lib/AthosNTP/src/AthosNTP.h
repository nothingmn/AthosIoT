//AthosNTP.h
#ifndef ATH_NTP_h
#define ATH_NTP_h

#include <WiFiUdp.h>
#include <NTPClient.h>

class AthosNTP {
    public:
        AthosNTP();
        void NTP_Loop(void);
        void NTP_Setup(void);
        long NTP_getEpochTime(void);
        
    private:
        void setTime(void);
        void setTime(long secondsSinceEpoc);
        void NTP_updateTime(void) ;
        long utcOffsetInSeconds = 0;
        char* ntp_server;
        int NTP_max_diff;
        WiFiUDP ntpUDP;
        int NTP_last;

};
#endif
