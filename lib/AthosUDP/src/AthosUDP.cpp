//AthosUDP.cpp

#include "AthosUDP.h"
#include "AthosNTP.h"
#include "AthosHelpers.h"
#include "AthosEEPROM.h"
#include <ArduinoLog.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

int broadcastPort = 3000;
int udpListenPort = 3001;

IPAddress broadcastIp(255, 255, 255, 255);
WiFiUDP broadcastUDP;
WiFiUDP listenUDP;

String udp_deviceId;
StorageValues _udp_config;
AthosHelpers _udp_helpers;
AthosEEPROM _udp_eeprom;
AthosNTP _udp_ntp;

bool UDP_configComplete = false;
void AthosUDP::listenForUDPMessages(void)
{
    // if there's data available, read a packet
    char packetBuffer[1500]; //buffer to hold incoming packet
    int packetSize = listenUDP.parsePacket();
    if (packetSize)
    {
      IPAddress remoteIp = listenUDP.remoteIP();
      Log.trace("Received UDP packet of size %d, from:%s", packetSize, remoteIp.toString().c_str());

      // read the packet into packetBufffer
      int len = listenUDP.read(packetBuffer, packetSize);
      if (len > 0)
      {
        packetBuffer[len] = 0;
      }
      Log.trace("Contents:");
      Log.trace(packetBuffer);

      StaticJsonDocument<512> doc;
      deserializeJson(doc, packetBuffer);

      _udp_config.mqttRelayTopic = doc["mqtt"]["relay"].as<String>();
      _udp_config.mqttPingTopic = doc["mqtt"]["ping"].as<String>();
      _udp_config.mqttSensorTopic = doc["mqtt"]["sensor"].as<String>();
      _udp_config.mqttMotionTopic = doc["mqtt"]["motion"].as<String>();
      _udp_config.mqttServer = doc["mqtt"]["server"].as<String>();
      _udp_config.mqttUsername = doc["mqtt"]["username"].as<String>();
      _udp_config.mqttPassword = doc["mqtt"]["password"].as<String>();
      _udp_config.mqttPort = doc["mqtt"]["port"].as<String>();
      _udp_config.ssid = doc["wifi"]["ssid"].as<String>();
      _udp_config.password = doc["wifi"]["password"].as<String>();
      
      Log.trace("Received MQTT Payload:");
      Log.trace(_udp_config.mqttServer.c_str());
      Log.trace(_udp_config.mqttSensorTopic.c_str());
      _udp_eeprom.writeEEPROMData(_udp_config);
      delay(100);
      UDP_configComplete = true;
      ESP.reset();
      delay(100);
    } 
  }


long last_udp_broadcast = 0;
void AthosUDP::publishUDP(void)
{
  long ts = _udp_ntp.NTP_getEpochTime();
  long diff = abs(last_udp_broadcast - ts);
  
  if (diff > 10)
  {
    Log.trace("Broadcasting myself...");
    broadcastUDP.beginPacket(broadcastIp, broadcastPort);

    StaticJsonDocument<200> doc;
    doc["ts"] = ts;
    doc["deviceid"] = udp_deviceId;
    String json;
    serializeJson(doc, json);
    broadcastUDP.print(json);
    broadcastUDP.endPacket();
    last_udp_broadcast = ts;    
  }
}
StorageValues AthosUDP::UDP_Setup(String DeviceId, StorageValues rootConfig)
{
  udp_deviceId = DeviceId;
  _udp_config = rootConfig;

  if(!_udp_config.mqttServer || !_udp_config.mqttSensorTopic || _udp_config.mqttServer == "null" || _udp_config.mqttServer == "") {
    Log.trace("No MQTT Data on file, UDP broadcast required");

    listenUDP.begin(udpListenPort);
    broadcastUDP.begin(broadcastPort);

    while(!UDP_configComplete) {
      publishUDP();
      listenForUDPMessages();
      delay(1000);
    }

  } else {
    Log.trace("MQTT Data on file, NO UDP broadcast required");
  }

  return _udp_config;
}

void AthosUDP::UDP_Loop(void)
{
    //will need to check back with the server via UDP for any changes in config
    //once every minute
}
/*
  Constructor
*/
AthosUDP::AthosUDP()
{
}

AthosUDP::AthosUDP(AthosHelpers helpers, AthosNTP ntp, AthosEEPROM eeprom) {
  _udp_helpers = helpers;
  _udp_ntp = ntp;
  _udp_eeprom = eeprom;
}

