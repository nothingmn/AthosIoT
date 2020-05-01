#ifdef ATH_NEOPIXEL

#ifndef ATH_NEOPIXEL_
#define ATH_NEOPIXEL_

#include "NeoPixelEffects.h"

#ifdef ATH_MQTT
  #include "AthosMQTT.h"
#endif 

#include <AthosHelpers.h>
#include <PubSubClient.h>
#include <ArduinoLog.h>
#include <Adafruit_NeoPixel.h>


int PixelPin = D2;
int PixelCount = 24;
// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PixelCount, PixelPin, NEO_RGBW + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


PubSubClient _NeoPixel_mqtt_client;
String _NeoPixel_deviceId;
StorageValues _NeoPixel_config;
int _NeoPixel_loop_delay;

void NeoPixel_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
  _NeoPixel_mqtt_client = mqtt_client;
  _NeoPixel_deviceId = deviceId;
  _NeoPixel_config = rootConfig;
  _NeoPixel_loop_delay = loop_delay;

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}



bool NeoPixel_MQTT_Received(String topic, String json) {
    
    if(topic.endsWith("/" + _NeoPixel_deviceId)) {

      Log.trace("NeoPixel_MQTT_Received  \nTopic:%s\nPayload:%s", topic.c_str(), json.c_str());
      
      //matched to the current device.  now do something with the JSON payload;
      StaticJsonDocument<255> readDoc;
      deserializeJson(readDoc, json);
      String command = readDoc["command"].as<String>();
      int pin = readDoc["pin"].as<int>();

    if(command == "allon") {
        Log.trace("allon");
        PixelEffect_on(strip);
        Log.trace("PixelEffect_on done");
        return true;
    } else if(command == "alloff") {
        Log.trace("alloff");
        PixelEffect_off(strip);
        Log.trace("PixelEffect_off done");
        return true;
    } else if (command == "dim") {
        Log.trace("dim");
        int level = readDoc["level"].as<int>();
        Log.trace("level %i", level);
        PixelEffect_dim(strip, level);        
        Log.trace("PixelEffect_dim");
        return true;
    } else if(command == "random") {
        Log.trace("random");
        PixelEffect_random(strip);
        Log.trace("random done");
        return true;
     } else if(command == "rainbowFade2White") {
        Log.trace("rainbowFade2White");
        int wait = readDoc["wait"].as<int>();
        Log.trace("wait %i", wait);
        int rainbowLoops = readDoc["rainbowLoops"].as<int>();
        Log.trace("rainbowLoops %i", rainbowLoops);
        int whiteLoops = readDoc["whiteLoops"].as<int>();
        Log.trace("whiteLoops %i", whiteLoops);
        PixelEffect_rainbowFade2White(strip, wait, rainbowLoops, whiteLoops);
        Log.trace("PixelEffect_rainbowFade2White done");
        return true;
      } else if(command == "color") {
        Log.trace("color");
        uint32_t r = readDoc["r"].as<uint32_t>();
        Log.trace("r %i", r);
        uint32_t g = readDoc["g"].as<uint32_t>();
        Log.trace("g %i", g);
        uint32_t b = readDoc["b"].as<uint32_t>();
        Log.trace("b %i", b);
        uint32_t w = readDoc["w"].as<uint32_t>();
        Log.trace("w %i", w);
        int wait = readDoc["wait"].as<int>();
        Log.trace("wait %i", wait);
        PixelEffect_fillColor(strip, r, g, b, w, wait);
        Log.trace("PixelEffect_fillColor done");
        return true;
      }  else if(command == "pulseWhite") {
        Log.trace("pulseWhite");
        int wait = readDoc["wait"].as<int>();
        Log.trace("wait %i", wait);
        int pulses = readDoc["pulses"].as<int>();
        Log.trace("pulses %i", pulses);
        PixelEffect_pulseWhite(strip, wait, pulses);
        Log.trace("PixelEffect_pulseWhite done");
        return true;
      } else if(command == "pulseWhiteWithColor") {
        Log.trace("pulseWhiteWithColor");

        uint32_t r = readDoc["r"].as<uint32_t>();
        Log.trace("r %i", r);
        uint32_t g = readDoc["g"].as<uint32_t>();
        Log.trace("g %i", g);
        uint32_t b = readDoc["b"].as<uint32_t>();
        Log.trace("b %i", b);

        int wait = readDoc["wait"].as<int>();        
        Log.trace("wait %i", wait);
        int pulses = readDoc["pulses"].as<int>();
        Log.trace("pulses %i", pulses);
        PixelEffect_pulseWhiteWithColor(strip, r, g, b, wait, pulses);
        Log.trace("PixelEffect_pulseWhiteWithColor done");
        return true;
      }   else if(command == "rainbow") {
        Log.trace("rainbow");
        int wait = readDoc["wait"].as<int>();
        Log.trace("wait %i", wait);
        int rainbowLoops = readDoc["rainbowLoops"].as<int>();
        Log.trace("rainbowLoops %i", rainbowLoops);
        PixelEffect_rainbow(strip, rainbowLoops, wait);
        Log.trace("PixelEffect_rainbow done");
        return true;
      }   else if(command == "whiteOverRainbow") {
        Log.trace("whiteOverRainbow");
        int whiteSpeed = readDoc["whiteSpeed"].as<int>();
        Log.trace("whiteSpeed %i", whiteSpeed);
        int whiteLength = readDoc["whiteLength"].as<int>();
        Log.trace("wawhiteLengthit %i", whiteLength);
        PixelEffect_whiteOverRainbow(strip, whiteSpeed, whiteLength);
        Log.trace("PixelEffect_whiteOverRainbow done");
        return true;
      }  
    return false;
    }
}

void NeoPixel_CheckIn()
{
  Log.trace("NeoPixel_CheckIn");
  int ts = NTP_getEpochTime();
  
  String csv = String("NeoPixel," + getVersion() + "," + ts + "," + PixelPin + "," + PixelCount + "," +  _NeoPixel_deviceId);
  const char* payload = csv.c_str();
  const char* topic = _NeoPixel_config.mqttSensorTopic.c_str();
  Serial.printf("Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());
  Log.trace("Topic:%s\nPayload:%s\nLength:%i\n",topic, payload, csv.length());

  if (!_NeoPixel_mqtt_client.publish(topic, payload))
  {
    Log.trace("NeoPixel Checkin Data to MQTT Failed. Packet > 128?");
  }
  MQTTTransmitLed();
}

long NeoPixel_last = 0;
long NeoPixel_max_diff = 60;
void NeoPixel_Loop()
{
  long now = NTP_getEpochTime();
  long diff = abs(now - NeoPixel_last);  
  if(diff > NeoPixel_max_diff || NeoPixel_last == 0) {
    NeoPixel_last = now;
    NeoPixel_CheckIn();
  }
}

#endif
#endif