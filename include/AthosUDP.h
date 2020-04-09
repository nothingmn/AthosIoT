#ifndef ATH_UDP
#define ATH_UDP

#include "AthosHelpers.h"
#include <ArduinoJson.h>
#include <ArduinoLog.h>
#include <ESP8266WiFi.h>
#include "Arduino.h"

String udp_deviceId;
StorageValues _udp_config;
bool UDP_configComplete = false;
int broadcastPort = 3000;

WiFiUDP broadcastUDP;
IPAddress broadcastIP(255,255,255,255);
WiFiUDP listenUDP;
int udpListenPort = 3001;

void listenForUDPMessages()
{
    // if there's data available, read a packet
    char packetBuffer[512]; //buffer to hold incoming packet
    Log.trace("Listen UDP");
    int packetSize = listenUDP.parsePacket();
    Log.trace("Parsed UDP");

    if (packetSize)
    {
      IPAddress remoteIp = listenUDP.remoteIP();
      Log.trace("Received UDP packet of size %d, from:%s", packetSize, remoteIp.toString().c_str());

      // read the packet into packetBufffer
      int len = listenUDP.read(packetBuffer, packetSize);
      Log.trace("recieved packet from udp, len::%i", len);
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
      writeEEPROMData(_udp_config);
      delay(100);
      UDP_configComplete = true;
      Log.trace("UDP is causing a reset...");
      ESP.reset();
      delay(100);
    } 
  }



long last_udp_broadcast = 0;
void publishUDP()
{
  long ts = NTP_getEpochTime();
  long diff = abs(last_udp_broadcast - ts);
  
  if (diff > 10)
  {

    const char* deviceid = udp_deviceId.c_str();
    Log.trace("Starting to Broadcasting myself...%s", deviceid);
    broadcastUDP.beginPacket(broadcastIP, broadcastPort);
    Log.trace("printing...%s", deviceid);
    broadcastUDP.print(deviceid);
    Log.trace("Ending packet...");
    broadcastUDP.endPacket();
    Log.trace("Broadcasted myself...");
    last_udp_broadcast = ts;    
  }
}
StorageValues UDP_Setup(String DeviceId, StorageValues rootConfig)
{
  udp_deviceId = DeviceId;
  _udp_config = rootConfig;

  if(!_udp_config.mqttServer || !_udp_config.mqttSensorTopic || _udp_config.mqttServer == "null" || _udp_config.mqttServer == "") {
    Log.trace("No MQTT Data on file, UDP broadcast required");

    broadcastUDP.begin(broadcastPort);
    listenUDP.begin(udpListenPort);

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

void UDP_Loop()
{
    //will need to check back with the server via UDP for any changes in config
    //once every minute
}

#endif