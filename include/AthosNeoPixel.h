#ifdef ATH_NEOPIXEL

#ifndef ATH_NEOPIXEL_
#define ATH_NEOPIXEL_

#include <AthosHelpers.h>
#include <PubSubClient.h>
#include <ArduinoLog.h>

#ifdef ATH_MQTT
  #include "AthosMQTT.h"
#endif 

#include <Adafruit_NeoPixel.h>
int PixelPin = D0;
int PixelCount = 18;
// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PixelCount, PixelPin, NEO_GRB + NEO_KHZ800);
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


// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void Pixel_rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}


bool NeoPixel_MQTT_Received(String topic, String json) {
  
  if(topic.endsWith("/" + _NeoPixel_deviceId)) {

    Log.trace("NeoPixel_MQTT_Received  \nTopic:%s\nPayload:%s", topic.c_str(), json.c_str());
    
    //matched to the current device.  now do something with the JSON payload;
    StaticJsonDocument<255> readDoc;
    deserializeJson(readDoc, json);
    String command = readDoc["command"].as<String>();

    if(command == "on" || command == "off" || command == "allon" || command == "alloff") {
      int pin = readDoc["pin"].as<int>();
      Log.trace("NeoPixel_MQTT COMMAND STARTING:\npin:%i\ncommand:%s", pin, command.c_str());
      Pixel_rainbow(10);
      Log.trace("NeoPixel_MQTT COMMAND COMPLETED:\npin:%i\ncommand:%s", pin, command.c_str());
      return true;
    }
  }
  return false;
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