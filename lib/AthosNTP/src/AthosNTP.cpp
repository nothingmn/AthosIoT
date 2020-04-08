//AthosNTP.cpp
#include "AthosNTP.h"
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

long AthosNTP::NTP_getEpochTime(void) {
  return timeClient.getEpochTime();
}
void AthosNTP::NTP_updateTime(void) {
  while(!timeClient.update()) {
    Log.trace("Failed to get an updated ntp time, will try again");
    delay(1000);
  }
}
void AthosNTP::setTime(long secondsSinceEpoc) {
    time_t t = secondsSinceEpoc;
    //struct tm tm = *localtime(&t);
    Log.trace("Setting time: %i", secondsSinceEpoc);
    struct timeval now = { .tv_sec = t };
    settimeofday(&now, NULL);
    Log.trace("time of day setup complete");
}
void AthosNTP::setTime(void) {
    setTime(NTP_getEpochTime());    
}

void AthosNTP::NTP_Setup(void)
{
  timeClient.begin();
  NTP_updateTime();
  setTime();

}
int NTP_last = 0;
void AthosNTP::NTP_Loop(void)
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

/*
  Constructor
*/
AthosNTP::AthosNTP()
{
}

