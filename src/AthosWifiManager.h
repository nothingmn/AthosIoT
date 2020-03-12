#ifdef ATH_WIFIMANAGER
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
  Serial.println("Waiting for Wifi to connect");
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.print("Local IP: ");
      Serial.println(WiFi.localIP());      
      return true;
    }
    delay(500);
    Serial.print("*");
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
}



void createWebServer()
{
    // Start the server
    WIFI_server.begin();
    Serial.println("Server started");

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
      if (qsid.length() > 0 && qpass.length() > 0) {
        _wifi_config.ssid = qsid;
        _wifi_config.password = qpass;
        writeEEPROMData(_wifi_config);
        WIFI_content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        WIFI_statusCode = 200;
        reboot = true;
      } else {
        WIFI_content = "{\"Error\":\"404 not found\"}";
        WIFI_statusCode = 404;
        Serial.println("Sending 404");
      }
      WIFI_server.sendHeader("Access-Control-Allow-Origin", "*");
      WIFI_server.send(WIFI_statusCode, "application/json", WIFI_content);
      if(reboot) {
        Serial.println("Restarting device so the new settings can take place");
        delay(1000);
        ESP.restart();
      }

    });
 }


void launchWeb()
{
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi connected");
    }
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("SoftAP IP: ");
    Serial.println(WiFi.softAPIP());
    createWebServer();
}

void setupAP(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
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
  Serial.println("softap");
  launchWeb();
  Serial.println("over");
}


StorageValues WifiManager_Setup(String deviceId, StorageValues rootConfig)
{
  _deviceId = deviceId;
  _wifi_config = rootConfig;
  _wifiAPName = "ESP_" + _deviceId;
  
  WiFi.disconnect();
  
  WiFi.begin(_wifi_config.ssid.c_str(), _wifi_config.password.c_str());
  if (testWifi())
  {
    Serial.println("Succesfully Connected!!!");
    return _wifi_config;
  }
  else
  {
    Serial.println("Turning the HotSpot On");
    launchWeb();
    setupAP();// Setup HotSpot
  }

  Serial.println();
  Serial.println("Waiting.");
  
  while ((WiFi.status() != WL_CONNECTED))
  {
    delay(100);
    WIFI_server.handleClient();
  }
  return _wifi_config;
}

void WifiManager_Loop()
{

}


#endif
