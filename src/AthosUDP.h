#ifdef ATH_UDP
#include <ArduinoJson.h>

int broadcastPort = 3000;
int udpListenPort = 3001;

IPAddress broadcastIp(255, 255, 255, 255);
WiFiUDP broadcastUDP;
WiFiUDP listenUDP;

String udp_deviceId;
StorageValues _udp_config;

bool UDP_configComplete = false;
void listenForUDPMessages()
{
    // if there's data available, read a packet
    char packetBuffer[255]; //buffer to hold incoming packet
    int packetSize = listenUDP.parsePacket();
    if (packetSize)
    {
      Serial.print("Received packet of size ");
      Serial.println(packetSize);
      Serial.print("From ");
      IPAddress remoteIp = listenUDP.remoteIP();
      Serial.print(remoteIp);
      Serial.print(", port ");
      Serial.println(listenUDP.remotePort());

      // read the packet into packetBufffer
      int len = listenUDP.read(packetBuffer, 255);
      if (len > 0)
      {
        packetBuffer[len] = 0;
      }
      Serial.println("Contents:");
      Serial.println(packetBuffer);

      StaticJsonDocument<1024> doc;
      deserializeJson(doc, packetBuffer);

      _udp_config.mqttRelayTopic = doc["mqtt"]["relay"].as<String>();
      _udp_config.mqttCapsTopic = doc["mqtt"]["caps"].as<String>();
      _udp_config.mqttSensorTopic = doc["mqtt"]["sensor"].as<String>();
      _udp_config.mqttServer = doc["mqtt"]["server"].as<String>();
      _udp_config.mqttUsername = doc["mqtt"]["username"].as<String>();
      _udp_config.mqttPassword = doc["mqtt"]["password"].as<String>();
      _udp_config.mqttPort = doc["mqtt"]["port"].as<String>();
      Serial.print("Received MQTT Payload:");
      Serial.println(_udp_config.mqttServer);
      Serial.println(_udp_config.mqttSensorTopic);
      writeEEPROMData(_udp_config);
      UDP_configComplete = true;
      ESP.restart();
    } 
  }


long last_udp_broadcast = 0;
void publishUDP()
{
  long ts = NTP_getEpochTime();
  long diff = abs(last_udp_broadcast - ts);
  
  if (diff > 10)
  {
    Serial.println("Broadcasting myself...");
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
StorageValues UDP_Setup(String DeviceId, StorageValues rootConfig)
{
  udp_deviceId = DeviceId;
  _udp_config = rootConfig;

  if(!_udp_config.mqttServer || !_udp_config.mqttSensorTopic || _udp_config.mqttServer == "null" || _udp_config.mqttServer == "") {
    Serial.println("No MQTT Data on file, UDP broadcast required");

    listenUDP.begin(udpListenPort);
    broadcastUDP.begin(broadcastPort);

    while(!UDP_configComplete) {
      publishUDP();
      listenForUDPMessages();
      delay(1000);
    }

  } else {
    Serial.println("MQTT Data on file, NO UDP broadcast required");
  }

  return _udp_config;
}

void UDP_Loop()
{

}

#endif
