#ifndef ATH_HTTP
#define ATH_HTTP

#include "AthosHelpers.h"
#include "AthosEEPROM.h"
#include "Arduino.h"
#include <ArduinoJson.h>
#include <ArduinoLog.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

String http_deviceId;
StorageValues _http_config;

#ifndef ATH_HELPERS
#include "AthosHelpers.h"
#endif

void HTTP_ScrapeConfig(void)
{
    // WiFiClient client;
    // if (client.connect(WiFi.gatewayIP(), 1880))
    // {
    //     // we are connected to the host!
    //     client.print(String("GET /configure?deviceid=" + http_deviceId) + " HTTP/1.1\r\n" +
    //                  "Host: " + WiFi.gatewayIP().toString() + "\r\n" +
    //                  "Connection: close\r\n" +
    //                  "\r\n");

    //     String json = String();
    //     while (client.connected() || client.available())
    //     {
    //         if (client.available())
    //         {
    //             client.av
    //                 String line = client.readStringUntil('\n');
    //             json += line;
    //         }
    //     }
    // }
    // else
    // {
    //     // connection failure
    //     Log.error("Could not connect to AthosIoT Node-Red server");
    //     return;
    // }
    //String _json = String(json.c_str());

    HTTPClient http;
    http.begin("http://" + WiFi.gatewayIP().toString() + ":1880/configure?deviceid=" + http_deviceId);	
    http.GET();
    String _json = http.getString();

    StaticJsonDocument<512> doc;
    deserializeJson(doc, _json);
    const char *world = doc["mqtt"]["server"];
    Log.trace("%s", world);

    _http_config.mqttRelayTopic = doc["mqtt"]["relay"].as<String>();
    _http_config.mqttPingTopic = doc["mqtt"]["ping"].as<String>();
    _http_config.mqttSensorTopic = doc["mqtt"]["sensor"].as<String>();
    _http_config.mqttMotionTopic = doc["mqtt"]["motion"].as<String>();
    _http_config.mqttServer = doc["mqtt"]["server"].as<String>();
    _http_config.mqttUsername = doc["mqtt"]["username"].as<String>();
    _http_config.mqttPassword = doc["mqtt"]["password"].as<String>();
    _http_config.mqttPort = doc["mqtt"]["port"].as<String>();

      String http_ssid = doc["wifi"]["ssid"].as<String>();
      String http_password = doc["wifi"]["password"].as<String>();
      if(http_ssid != "null" && http_ssid != "") _http_config.ssid = http_ssid;
      if(http_password != "null" && http_password != "") _http_config.password = http_password;

    _http_config.ssid = doc["wifi"]["ssid"].as<String>();
    _http_config.password = doc["wifi"]["password"].as<String>();

    Log.trace("Received HTTP Payload:%s, %s", _http_config.mqttServer.c_str(), _http_config.mqttSensorTopic.c_str());
    Log.trace("---------------------------------------------");
    http.end();

    writeEEPROMData(_http_config);
    delay(100);
    Log.trace("HTTP is causing a reset...");
    ESP.restart();
    delay(100);
}
StorageValues HTTP_Setup(String DeviceId, StorageValues rootConfig)
{
    http_deviceId = DeviceId;
    _http_config = rootConfig;

    if (!_http_config.mqttServer || !_http_config.mqttSensorTopic || _http_config.mqttServer == "null" || _http_config.mqttServer == "")
    {
        Log.trace("No MQTT Data on file, HTTP scrape required");

        while (true)
        {
            HTTP_ScrapeConfig();
            delay(1000);
        }
    }
    else
    {
        Log.trace("MQTT Data on file, NO UDP broadcast required");
    }

    return _http_config;
}

void HTTP_Loop()
{
}

#endif
