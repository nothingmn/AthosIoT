#ifdef ATH_NEOPIXEL

#ifndef ATH_NEOPIXEL_
#define ATH_NEOPIXEL_

#ifdef ATH_MQTT
#include "AthosMQTT.h"
#endif
//#include "HashMap.h"
#include "IR24ButtonRemote.h"
#include <AthosHelpers.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoLog.h>
#include <WS2812FX.h>
#include <HashMap.h>
CreateHashMap(fxMappings, String, int, 55);
int PixelPin = D2;
int PixelCount = 24;
int NeoPixel_Last_Brightness = 255;

// Declare our NeoPixel strip object:
WS2812FX ws2812fx = WS2812FX(PixelCount, PixelPin, NEO_RGBW + NEO_KHZ800);
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

  fxMappings["FX_MODE_STATIC"] = FX_MODE_STATIC;
  fxMappings["FX_MODE_BLINK"] = FX_MODE_BLINK;
  fxMappings["FX_MODE_BREATH"] = FX_MODE_BREATH;
  fxMappings["FX_MODE_COLOR_WIPE"] = FX_MODE_COLOR_WIPE;
  fxMappings["FX_MODE_COLOR_WIPE_INV"] = FX_MODE_COLOR_WIPE_INV;
  fxMappings["FX_MODE_COLOR_WIPE_REV"] = FX_MODE_COLOR_WIPE_REV;
  fxMappings["FX_MODE_COLOR_WIPE_REV_INV"] = FX_MODE_COLOR_WIPE_REV_INV;
  fxMappings["FX_MODE_COLOR_WIPE_RANDOM"] = FX_MODE_COLOR_WIPE_RANDOM;
  fxMappings["FX_MODE_RANDOM_COLOR"] = FX_MODE_RANDOM_COLOR;
  fxMappings["FX_MODE_SINGLE_DYNAMIC"] = FX_MODE_SINGLE_DYNAMIC;
  fxMappings["FX_MODE_MULTI_DYNAMIC"] = FX_MODE_MULTI_DYNAMIC;
  fxMappings["FX_MODE_RAINBOW"] = FX_MODE_RAINBOW;
  fxMappings["FX_MODE_RAINBOW_CYCLE"] = FX_MODE_RAINBOW_CYCLE;
  fxMappings["FX_MODE_SCAN"] = FX_MODE_SCAN;
  fxMappings["FX_MODE_DUAL_SCAN"] = FX_MODE_DUAL_SCAN;
  fxMappings["FX_MODE_FADE"] = FX_MODE_FADE;
  fxMappings["FX_MODE_THEATER_CHASE"] = FX_MODE_THEATER_CHASE;
  fxMappings["FX_MODE_THEATER_CHASE_RAINBOW"] = FX_MODE_THEATER_CHASE_RAINBOW;
  fxMappings["FX_MODE_RUNNING_LIGHTS"] = FX_MODE_RUNNING_LIGHTS;
  fxMappings["FX_MODE_TWINKLE"] = FX_MODE_TWINKLE;
  fxMappings["FX_MODE_TWINKLE_RANDOM"] = FX_MODE_TWINKLE_RANDOM;
  fxMappings["FX_MODE_TWINKLE_FADE"] = FX_MODE_TWINKLE_FADE;
  fxMappings["FX_MODE_TWINKLE_FADE_RANDOM"] = FX_MODE_TWINKLE_FADE_RANDOM;
  fxMappings["FX_MODE_SPARKLE"] = FX_MODE_SPARKLE;
  fxMappings["FX_MODE_FLASH_SPARKLE"] = FX_MODE_FLASH_SPARKLE;
  fxMappings["FX_MODE_HYPER_SPARKLE"] = FX_MODE_HYPER_SPARKLE;
  fxMappings["FX_MODE_STROBE"] = FX_MODE_STROBE;
  fxMappings["FX_MODE_STROBE_RAINBOW"] = FX_MODE_STROBE_RAINBOW;
  fxMappings["FX_MODE_MULTI_STROBE"] = FX_MODE_MULTI_STROBE;
  fxMappings["FX_MODE_BLINK_RAINBOW"] = FX_MODE_BLINK_RAINBOW;
  fxMappings["FX_MODE_CHASE_WHITE"] = FX_MODE_CHASE_WHITE;
  fxMappings["FX_MODE_CHASE_COLOR"] = FX_MODE_CHASE_COLOR;
  fxMappings["FX_MODE_CHASE_RANDOM"] = FX_MODE_CHASE_RANDOM;
  fxMappings["FX_MODE_CHASE_RAINBOW"] = FX_MODE_CHASE_RAINBOW;
  fxMappings["FX_MODE_CHASE_FLASH"] = FX_MODE_CHASE_FLASH;
  fxMappings["FX_MODE_CHASE_FLASH_RANDOM"] = FX_MODE_CHASE_FLASH_RANDOM;
  fxMappings["FX_MODE_CHASE_RAINBOW_WHITE"] = FX_MODE_CHASE_RAINBOW_WHITE;
  fxMappings["FX_MODE_CHASE_BLACKOUT"] = FX_MODE_CHASE_BLACKOUT;
  fxMappings["FX_MODE_CHASE_BLACKOUT_RAINBOW"] = FX_MODE_CHASE_BLACKOUT_RAINBOW;
  fxMappings["FX_MODE_COLOR_SWEEP_RANDOM"] = FX_MODE_COLOR_SWEEP_RANDOM;
  fxMappings["FX_MODE_RUNNING_COLOR"] = FX_MODE_RUNNING_COLOR;
  fxMappings["FX_MODE_RUNNING_RED_BLUE"] = FX_MODE_RUNNING_RED_BLUE;
  fxMappings["FX_MODE_RUNNING_RANDOM"] = FX_MODE_RUNNING_RANDOM;
  fxMappings["FX_MODE_LARSON_SCANNER"] = FX_MODE_LARSON_SCANNER;
  fxMappings["FX_MODE_COMET"] = FX_MODE_COMET;
  fxMappings["FX_MODE_FIREWORKS"] = FX_MODE_FIREWORKS;
  fxMappings["FX_MODE_FIREWORKS_RANDOM"] = FX_MODE_FIREWORKS_RANDOM;
  fxMappings["FX_MODE_MERRY_CHRISTMAS"] = FX_MODE_MERRY_CHRISTMAS;
  fxMappings["FX_MODE_HALLOWEEN"] = FX_MODE_HALLOWEEN;
  fxMappings["FX_MODE_FIRE_FLICKER"] = FX_MODE_FIRE_FLICKER;
  fxMappings["FX_MODE_FIRE_FLICKER_SOFT"] = FX_MODE_FIRE_FLICKER_SOFT;
  fxMappings["FX_MODE_FIRE_FLICKER_INTENSE"] = FX_MODE_FIRE_FLICKER_INTENSE;
  fxMappings["FX_MODE_CIRCUS_COMBUSTUS"] = FX_MODE_CIRCUS_COMBUSTUS;
  fxMappings["FX_MODE_BICOLOR_CHASE"] = FX_MODE_BICOLOR_CHASE;
  fxMappings["FX_MODE_TRICOLOR_CHASE"] = FX_MODE_TRICOLOR_CHASE;
  fxMappings["FX_MODE_ICU"] = FX_MODE_ICU;

  ws2812fx.init();
  ws2812fx.setBrightness(255);
  ws2812fx.setSpeed(200);
  ws2812fx.setMode(FX_MODE_RAINBOW_CYCLE);
  ws2812fx.start();
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void AllOff(void)
{
  Log.trace("alloff");
  NeoPixel_Last_Brightness = ws2812fx.getBrightness();
  ws2812fx.stop();
  ws2812fx.setBrightness(0);
  Log.trace("alloff done");
}
void AllOn(void)
{
  Log.trace("allon");
  ws2812fx.setBrightness(NeoPixel_Last_Brightness);
  ws2812fx.start();
  Log.trace("allon done");
}
void SetMode(int mode) {
  ws2812fx.setMode(mode);
  ws2812fx.start();
}
void SetColor(int r, int g, int b, int w) {
  ws2812fx.setColor(g, r, b, w);
  ws2812fx.setMode(FX_MODE_STATIC);
  ws2812fx.start();
}
void Control(int mode, int speed, int r, int g, int b, int w, int level)
{
  Log.trace("fx mode enabled start:%i", mode);
  ws2812fx.setSpeed(speed);
  ws2812fx.setBrightness(level);
  ws2812fx.setMode(mode);
  ws2812fx.start();
}
void NeoPixel_IR_Received(String _hex)
{
  int command = IR24_Parse(_hex);
  int level = ws2812fx.getBrightness();
  int mode = ws2812fx.getMode();

  switch (command)
  {
  case IR_COMMAND_ON:
    AllOn();
    break;
  case IR_COMMAND_OFF:
    AllOff();
    break;
  case IR_COMMAND_BRIUP:
    level = level + 25;
    if(level > 255) level = 255;
    ws2812fx.setBrightness(level);
    break;
  case IR_COMMAND_BRIDOWN:
    level = level - 25;
    if(level <= 0) level = 0;
    ws2812fx.setBrightness(level);
    break;
  case IR_COMMAND_R0:
    SetColor(255, 0, 0, 0);
    break;
  case IR_COMMAND_G0:
    SetColor(0, 255, 0, 0);
    break;
  case IR_COMMAND_B0:
    SetColor(0, 0, 255, 0);
    break;
  case IR_COMMAND_W0:
    SetColor(0, 0, 0, 255);
    break;
  
  case IR_COMMAND_R1:
    SetColor(204, 102, 0, 0);
    break;
  case IR_COMMAND_G1:
    SetColor(102, 204, 0, 0);
    break;
  case IR_COMMAND_B1:
    SetColor(0, 102, 204, 0);
    break;
  
  case IR_COMMAND_R2:
    SetColor(255, 128, 0, 0);
    break;
  case IR_COMMAND_G2:
    SetColor(0, 255, 255, 0);
    break;
  case IR_COMMAND_B2:
    SetColor(127, 0, 255, 0);
    break;
  
  case IR_COMMAND_R3:
    SetColor(255, 178, 102, 0);
    break;
  case IR_COMMAND_G3:
    SetColor(102, 178, 255, 0);
    break;
  case IR_COMMAND_B3:
    SetColor(178, 102, 255, 0);
    break;
  
  case IR_COMMAND_R4:
    SetColor(255, 255, 0, 0);
    break;
  case IR_COMMAND_G4:
    SetColor(102, 102, 255, 0);
    break;
  case IR_COMMAND_B4:
    SetColor(255, 51, 255, 0);
    break;
  
  
  case IR_COMMAND_FLASH:  
    SetMode(FX_MODE_COLOR_SWEEP_RANDOM);
    break;
  case IR_COMMAND_STROBE:
    SetMode(FX_MODE_FIREWORKS_RANDOM);
    break;
  case IR_COMMAND_FADE:
    mode++;
    ws2812fx.setMode(mode);
    break;
  case IR_COMMAND_SMOOTH:
    mode--;
    ws2812fx.setMode(mode);
    break;
  // case IR_COMMAND_UNKNOWN:
  //   break;
  default:
    Log.trace("IR not found:hex=%s", _hex.c_str());
    break;
  }
}

void NeoPixel_PIR_Received(int movement)
{
}

bool NeoPixel_MQTT_Received(String topic, String json)
{

  if (topic.endsWith("/" + _NeoPixel_deviceId))
  {

    Log.trace("NeoPixel_MQTT_Received  \nTopic:%s\nPayload:%s", topic.c_str(), json.c_str());

    //matched to the current device.  now do something with the JSON payload;
    StaticJsonDocument<255> readDoc;
    deserializeJson(readDoc, json);
    String command = readDoc["command"].as<String>();
    String fx = readDoc["fx"].as<String>();

    if (command == "allon")
    {
      AllOn();
      return true;
    }
    else if (command == "alloff")
    {
      AllOff();
      return true;
    }
    else if (command == "fx")
    {

      // {
      //   "command": "fx",
      //   "fx": "FX_MODE_MERRY_CHRISTMAS",
      //   "o" : "255,255,255,255"  //r,g,b,w
      //   "s" : 5000
      //   "l" : 255
      //}

      int level = readDoc["l"].as<int>();
      int speed = readDoc["s"].as<int>();
      String options = readDoc["o"].as<String>();

      String red = "0";
      String green = "0";
      String blue = "0";
      String white = "0";

      if (options != "")
      {
        red = getValue(options, ',', 0);
        green = getValue(options, ',', 1);
        blue = getValue(options, ',', 2);
        white = getValue(options, ',', 3);
      }

      int r = red.toInt();
      int g = green.toInt();
      int b = blue.toInt();
      int w = white.toInt();

      if (level == 0)
        level = 255;
      if (speed == 0)
        speed = 1000;

      Log.trace("red:%i,green:%i,blue:%i,white:%i,level:%i,speed:%i,", r, g, b, w, level, speed);

      int mode = fxMappings[fx];
      Control(mode, speed, r, g, b, w, level);
      return true;
    }
  }
  return false;
}

void NeoPixel_CheckIn()
{
  Log.trace("NeoPixel_CheckIn");
  int ts = NTP_getEpochTime();

  String csv = String("NeoPixel," + getVersion() + "," + ts + "," + PixelPin + "," + PixelCount + "," + _NeoPixel_deviceId);
  const char *payload = csv.c_str();
  const char *topic = _NeoPixel_config.mqttSensorTopic.c_str();
  Log.trace("Topic:%s\nPayload:%s\nLength:%i\n", topic, payload, csv.length());

  if (!_NeoPixel_mqtt_client.publish(topic, payload))
  {
    Log.trace("NeoPixel Checkin Data to MQTT Failed. Packet > 128?");
  }
  MQTTTransmitLed();
}

long NeoPixel_last = 0;
long NeoPixel_max_diff = 1000 * 60 * 10; //10 minutes
void NeoPixel_Loop()
{
  ws2812fx.service();

  long now = NTP_getEpochTime();
  long diff = abs(now - NeoPixel_last);
  if (diff > NeoPixel_max_diff || NeoPixel_last == 0)
  {
    NeoPixel_last = now;
    NeoPixel_CheckIn();
  }
}

#endif
#endif