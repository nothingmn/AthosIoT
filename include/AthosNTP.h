#ifdef ATH_NTP
#include <NTPClient.h>
#include <WiFiUdp.h>

//f time offset for PST
const long utcOffsetInSeconds = 0; //0 offset, lets keep it uTC for now
const char* ntp_server = "pool.ntp.org";
//1 = 1 second, since getEpocTime() returns down to the second
//1 * 60 = 60 seconds / 1 minute
//1 * 60 * 60 = 1 hour
//and so on
const int NTP_max_diff = 1 * 60 * 60;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntp_server, utcOffsetInSeconds);

void NTP_Setup()
{
  timeClient.begin();
}
long NTP_getEpochTime() {
  return timeClient.getEpochTime();
}
void NTP_updateTime() {
  timeClient.update(); 
}

int NTP_last = 0;
void NTP_Loop()
{
  int current = NTP_getEpochTime();
  int diff = abs(current - NTP_last);
  if(diff > NTP_max_diff) {
    Serial.println("updating NTP");
    NTP_updateTime();
    NTP_last = current;
  }
}

#endif
