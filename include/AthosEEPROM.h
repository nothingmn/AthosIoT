#include <EEPROM.h>
#include <ArduinoJson.h>

#ifndef ATH_LOG
  #include "AthosLog.h"
#endif
#ifndef ATH_HELPERS
  #include "AthosHelpers.h"
#endif

#define EEPROM_SIZE 2048
#define EEPROM_TERMINATER 0

void wipeEEPROM()
{
  log_info("Wiping EEPROM");
  EEPROM.begin(EEPROM_SIZE);
  for (int i = 0 ; i < EEPROM_SIZE ; i++) {
    EEPROM.put(i, 0);
  }
  EEPROM.commit();
  delay(1000);
  log_info("Wiped EEPROM");
}


StorageValues readEEPROMData() {

  StorageValues values;
  
  String content;
  EEPROM.begin(EEPROM_SIZE);
  for (int i = 0; i < EEPROM_SIZE; ++i)
  {
    char value = char(EEPROM.read(i));
    if(value != EEPROM_TERMINATER) {
      content += value;
    } else {
      break;
    }
  }

  if(content[0] == '{') {
    log_info("Read EEPROM %s", content.c_str());

    StaticJsonDocument<EEPROM_SIZE> readDoc;
    deserializeJson(readDoc, content);

    values.ssid =            readDoc["wifi"]["ssid"].as<String>();
    values.password =        readDoc["wifi"]["password"].as<String>();

    values.mqttServer =      readDoc["mqtt"]["server"].as<String>();
    values.mqttUsername =    readDoc["mqtt"]["username"].as<String>();
    values.mqttPassword =    readDoc["mqtt"]["password"].as<String>();
    values.mqttPort =        readDoc["mqtt"]["port"].as<String>();
    values.mqttSensorTopic = readDoc["mqtt"]["sensor"].as<String>();
    values.mqttCapsTopic = readDoc["mqtt"]["caps"].as<String>();
    values.mqttPingTopic = readDoc["mqtt"]["ping"].as<String>();
    values.mqttRelayTopic = readDoc["mqtt"]["relay"].as<String>();


  }

  return values;
}

void writeEEPROMData(StorageValues config) {

  wipeEEPROM();

  StaticJsonDocument<EEPROM_SIZE> writeDoc;
  writeDoc["wifi"]["ssid"] =      config.ssid;
  writeDoc["wifi"]["password"] =  config.password;
  
  writeDoc["mqtt"]["server"] =    config.mqttServer;
  writeDoc["mqtt"]["username"] =  config.mqttUsername;
  writeDoc["mqtt"]["password"] =  config.mqttPassword;
  writeDoc["mqtt"]["port"] =      config.mqttPort;
  writeDoc["mqtt"]["sensor"] =    config.mqttSensorTopic;
  writeDoc["mqtt"]["ping"] =      config.mqttPingTopic;
  writeDoc["mqtt"]["caps"] =      config.mqttCapsTopic;
  writeDoc["mqtt"]["relay"] =     config.mqttRelayTopic;


  String _json;
  serializeJson(writeDoc, _json);
  log_info("Writing EEPROM: %s", _json.c_str());

  _json += EEPROM_TERMINATER;

  EEPROM.begin(EEPROM_SIZE);
  for (long i = 0; i < _json.length()-1; ++i)
  {
    EEPROM.put(i, _json[i]);
  }
  EEPROM.commit();
  delay(1000);
}



StorageValues EEPROM_setup() {

  if(false) {
    wipeEEPROM();   
    log_info("EEPROM KILLED!"); 
  }
  return readEEPROMData();
}
