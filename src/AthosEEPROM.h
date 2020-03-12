#include <EEPROM.h>
#include <ArduinoJson.h>

#ifndef ATH_HELPERS
  #include "AthosHelpers.h"
#endif

#define EEPROM_SIZE 2048
#define EEPROM_TERMINATER 0

void wipeEEPROM()
{
  Serial.println("Wiping EEPROM");
  EEPROM.begin(EEPROM_SIZE);
  for (int i = 0 ; i < EEPROM_SIZE ; i++) {
    EEPROM.put(i, 0);
  }
  EEPROM.commit();
  delay(1000);
  Serial.println("Wiped EEPROM");
}


StorageValues readEEPROMData() {

  StorageValues values;
  
  String content;
  Serial.println("READ eeprom characters:");
  EEPROM.begin(EEPROM_SIZE);
  for (int i = 0; i < EEPROM_SIZE; ++i)
  {
    char value = char(EEPROM.read(i));
    Serial.print(value);
    if(value != EEPROM_TERMINATER) {
      content += value;
    } else {
      break;
    }
  }
  Serial.println("DONE Reading eeprom characters:");
  

  if(content[0] == '{') {

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
  writeDoc["mqtt"]["topic"] =     config.mqttSensorTopic;

  Serial.println("----------WRITE EEPROM MQTT Payload----------");
  Serial.print("SERVER:");
  Serial.println(config.mqttServer);
  Serial.print("TOPIC:");
  Serial.println(config.mqttSensorTopic);
  Serial.print("PORT:");
  Serial.println(config.mqttPort);
  Serial.println("----------WRITE EEPROM MQTT Payload----------");

  String _json;
  serializeJson(writeDoc, _json);

  Serial.println("WRITING eeprom document:");
  Serial.println(_json);
  _json += EEPROM_TERMINATER;

  EEPROM.begin(EEPROM_SIZE);
  for (int i = 0; i < _json.length(); ++i)
  {
    EEPROM.put(i, _json[i]);
  }
  EEPROM.commit();
  delay(1000);
  Serial.println("WROTE eeprom document:");
}



StorageValues EEPROM_setup() {

  if(false) {
    wipeEEPROM();   
    Serial.println("EEPROM KILLED!"); 
  }
  

  return readEEPROMData();

}
