#ifndef ATH_NTP
#define ATH_NTP

#include <ArduinoLog.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>

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

long NTP_getEpochTime() {
  return timeClient.getEpochTime();
}
void NTP_updateTime() {
  while(!timeClient.update()) {
    Log.trace("Failed to get an updated ntp time, will try again");
    delay(100);
  }
}
void setTime(long secondsSinceEpoc) {
    time_t t = secondsSinceEpoc;
    //struct tm tm = *localtime(&t);
    Log.trace("Setting time: %i", secondsSinceEpoc);
    struct timeval now = { .tv_sec = t };
    settimeofday(&now, NULL);
    Log.trace("time of day setup complete");
}
void setTime() {
    setTime(NTP_getEpochTime());    
}

void NTP_Setup()
{
  timeClient.begin();
  NTP_updateTime();
  setTime();

}
int NTP_last = 0;
void NTP_Loop()
{
  int current = NTP_getEpochTime();
  int diff = abs(current - NTP_last);
  if(diff > NTP_max_diff) {
    Log.trace("updating NTP");
    NTP_updateTime();
    //reset our clock every iteration
    setTime();
    NTP_last = current;
  }
}
#endif