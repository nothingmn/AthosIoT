#include <EEPROM.h>
#include <ArduinoJson.h>

#ifndef ATH_HELPERS
  #include "AthosHelpers.h"
#endif

#define EEPROM_SIZE 4096
#define EEPROM_TERMINATER '~'

void wipeEEPROM()
{
   for (int i = 0 ; i < EEPROM_SIZE ; i++) {
    EEPROM.write(i, 0);
  }
}


StorageValues readEEPROMData() {


  StorageValues values;
  
  String content;
  for (int i = 0; i < EEPROM_SIZE; ++i)
  {
    char value = char(EEPROM.read(i));
    if(value != EEPROM_TERMINATER) {
      content += value;
    } else {
      break;
    }
  }

  StaticJsonDocument<EEPROM_SIZE> readDoc;
  deserializeJson(readDoc, content);

  Serial.println("Read eeprom document:");
  Serial.println(content);
  
  values.ssid =            readDoc["wifi"]["ssid"].as<String>();
  values.password =        readDoc["wifi"]["password"].as<String>();

  values.mqttServer =      readDoc["mqtt"]["server"].as<String>();
  values.mqttUsername =    readDoc["mqtt"]["username"].as<String>();
  values.mqttPassword =    readDoc["mqtt"]["password"].as<String>();
  values.mqttPort =        readDoc["mqtt"]["port"].as<String>();
  values.mqttSensorTopic = readDoc["mqtt"]["topic"].as<String>();

  Serial.print("READEEPROM MQTT Payload:");
  Serial.println(values.mqttServer);
  Serial.println(values.mqttSensorTopic);
  Serial.println(values.mqttPort);

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
  writeDoc["mqtt"]["topic"] =     config.mqttSensorTopic;

  Serial.print("WRITEEEPROM MQTT Payload:");
  Serial.println(config.mqttServer);
  Serial.println(config.mqttSensorTopic);
  Serial.println(config.mqttPort);

  String _json;
  serializeJson(writeDoc, _json);

  Serial.println("writing eeprom document:");
  Serial.println(_json);
  _json += EEPROM_TERMINATER;
  for (int i = 0; i < _json.length(); ++i)
  {
    EEPROM.write(i, _json[i]);
  }
  EEPROM.commit();
  delay(1000);
}



StorageValues EEPROM_setup() {

  if(false) {
    wipeEEPROM();    
  }

  return readEEPROMData();

}
