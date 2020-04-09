#ifndef ATH_WIFIMANAGER
#define ATH_WIFIMANAGER

#include <ArduinoLog.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

String _deviceId;
StorageValues _wifi_config;
String _wifiAPName;
String WIFI_content;
ESP8266WebServer WIFI_server(80);
String WIFI_st;
int WIFI_statusCode;

//-------- Fuctions used for WiFi credentials saving and connecting to it which you do not need to change
bool testWifi(void)
{
  int c = 0;
  Log.trace("Waiting for Wifi to connect");
  while (c < 20)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
            
      Log.trace("Local IP: %s, RSSI:%i", WiFi.localIP().toString().c_str(), WiFi.RSSI());
      return true;
    }
    delay(500);
    Log.trace("*");
    c++;
  }
  Log.trace("Connect timed out");
  return false;
}

void createWebServer()
{
  // Start the server
  WIFI_server.begin();
  Log.trace("Server started");

  WIFI_server.on("/", []() {
    IPAddress ip = WiFi.softAPIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    WIFI_content = "<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
    WIFI_content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
    WIFI_content += ipStr;
    WIFI_content += "<p>";
    WIFI_content += WIFI_st;
    WIFI_content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
    WIFI_content += "</html>";
    WIFI_server.send(200, "text/html", WIFI_content);
  });
  WIFI_server.on("/scan", []() {
    //setupAP();
    IPAddress ip = WiFi.softAPIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

    WIFI_content = "<!DOCTYPE HTML>\r\n<html>go back";
    WIFI_server.send(200, "text/html", WIFI_content);
  });

  WIFI_server.on("/setting", []() {
    bool reboot = false;
    String qsid = WIFI_server.arg("ssid");
    String qpass = WIFI_server.arg("pass");
    if (qsid.length() > 0 && qpass.length() > 0)
    {
      _wifi_config.ssid = qsid;
      _wifi_config.password = qpass;
      writeEEPROMData(_wifi_config);
      WIFI_content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
      WIFI_statusCode = 200;
      reboot = true;
    }
    else
    {
      WIFI_content = "{\"Error\":\"404 not found\"}";
      WIFI_statusCode = 404;
      Log.trace("Sending 404");
    }
    WIFI_server.sendHeader("Access-Control-Allow-Origin", "*");
    WIFI_server.send(WIFI_statusCode, "application/json", WIFI_content);
    if (reboot)
    {
      Log.trace("Restarting device so the new settings can take place");
      delay(1000);
      ESP.restart();
    }
  });
}

void launchWeb()
{
  Log.trace("");
  if (WiFi.status() == WL_CONNECTED)
  {
    Log.trace("WiFi connected");
  }
  Log.trace("LocalIP:%s, SoftAP IP:%s", WiFi.localIP().toString().c_str(), WiFi.softAPIP().toString().c_str());
  createWebServer();
}

void setupAP(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Log.trace("scan done");
  if (n == 0)
    Log.trace("no networks found");
  else
  {
    Log.trace("%i networks found", n);
  }
  WIFI_st = "<ol>";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    WIFI_st += "<li>";
    WIFI_st += WiFi.SSID(i);
    WIFI_st += " (";
    WIFI_st += WiFi.RSSI(i);
    WIFI_st += ")";
    WIFI_st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    WIFI_st += "</li>";
  }
  WIFI_st += "</ol>";
  delay(100);
  WiFi.softAP(_wifiAPName, "");
  launchWeb();
}

StorageValues WifiManager_Setup(String deviceId, StorageValues rootConfig)
{
  WifiSetupStartLed();
  _deviceId = deviceId;
  _wifi_config = rootConfig;
  _wifiAPName = "ATH_" + _deviceId;

  WiFi.disconnect();

  WiFi.mode(WIFI_STA);
  WiFi.begin(_wifi_config.ssid.c_str(), _wifi_config.password.c_str());
  WiFi.setAutoReconnect(true);
  if (testWifi())
  {
    Log.trace("Succesfully Connected to AP--> %s by device: %s\n", _wifi_config.ssid.c_str(), _wifiAPName.c_str());
    return _wifi_config;
  }
  else
  {
    //no configured wifi, or the configured wifi connection failed
    //can we connect to the Athos Hub hidden wifi?
    Log.trace("Attempting to connect to the AthosIoT Hub...");
    WiFi.begin("AthosIoT", "88888888");

    if (testWifi())
    {
      Log.trace("Succesfully Connected to AP--> AthosIoT by device: %s, Gateway:%s",  _wifiAPName.c_str(), WiFi.gatewayIP().toString().c_str());
      
      return _wifi_config;
    }
    else
    {
      //still couldnt connect to the Hub Wifi....
      Log.trace("Could not connect to the AthosIoT Hub, Turning the HotSpot On");
      launchWeb();
      setupAP(); // Setup HotSpot
    }
  }

  Log.trace("Waiting for devices on AP--> %s", _wifiAPName.c_str());

  while ((WiFi.status() != WL_CONNECTED))
  {
    delay(100);
    WIFI_server.handleClient();
  }
  WifiSetupCompleteLed();
  return _wifi_config;
}

void EnsureWifiConnected()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Log.trace("EnsureWifiConnected");
    WiFi.setAutoReconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(_wifi_config.ssid.c_str(), _wifi_config.password.c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
      Log.trace("Connecting to WiFi..");
      delay(500);
      Log.trace(".");
    }
    Log.trace("Connected to the WiFi network");
  }
}
void WifiManager_Loop()
{
  //we should have never gotten past setup so we assume Wifi is already configured and ready
  EnsureWifiConnected();
}
#endif