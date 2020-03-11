#include <EEPROM.h>
#include <ArduinoJson.h>


/********************************************************************************************

                       EEPROM-Cleaner v1.2.0

                       Copyright (c) 2017 Helmut Stult (schinfo)

  https://www.esp8266.com/viewtopic.php?t=14710#
 ********************************************************************************************/

// sizeBytes being the number of bytes you want to use.
// It's defined with "#define sizeBytes"
// Size can be anywhere between 4 and 4096 bytes (Default for ESP8266_deauther = 4096)
#define sizeBytes 4096

void wipeEEPROM()
{
  // change it for lower or higher endByte (Default for ESP8266_deauther = 4096)
  // normaly it's the same as sizeBytes
#define endByte 4096

  // change it for lower or higher startByte (Default = 0)
#define startByte 0

  unsigned long ok = 0;
  unsigned long nok = 0;
  unsigned long tok = 0;


  Serial.println("**********************************************************************************************************");
  Serial.print("    Write a char(255) / hex(FF) from byte ");
  Serial.print(startByte);
  Serial.print(" to ");
  Serial.print(endByte - 1);
  Serial.print(" into the EEPROM with a defined size of ");
  Serial.print(sizeBytes);
  Serial.println(" Bytes");
  Serial.println("**********************************************************************************************************");
  Serial.println("             testing EEPROM for written bytes");
  for (int i = startByte; i < endByte; ++i)
  {
    if (EEPROM.read(i) == 255) {
      ++ok;
    } else {
      ++nok;
    }
  }

  Serial.printf("               empty bytes: %6d\r\n", ok);
  Serial.printf("           not empty bytes: %6d\r\n", nok);
  Serial.println("**********************************************************************************************************");
  Serial.println("**********************************************************************************************************");
  Serial.println("             Start clearing EEPROM... - Please wait!!!");
  Serial.println("**********************************************************************************************************");

  // write a char(255) / hex(FF) from startByte until endByte into the EEPROM
  for (int i = startByte; i < endByte; ++i) {
    EEPROM.write(i, -1);
  }

  EEPROM.commit();

  delay(1000);

  Serial.println("             testing EEPROM for clearing");

  String test;
  for (int i = startByte; i < endByte; ++i)
  {
    if (EEPROM.read(i) == 255) {
      ++tok;
    }
  }
  Serial.println("**********************************************************************************************************");
  if (tok = endByte - startByte) {
    Serial.println("             EEPROM killed correctly");
  } else
    Serial.println("             EEPROM not killed - ERROR !!!");
  Serial.println("**********************************************************************************************************");
}

StorageValues readEEPROMData() {

  StorageValues values;
  
  String content;
  for (int i = 0; i < sizeBytes; ++i)
  {
    content += char(EEPROM.read(i));
  }

  StaticJsonDocument<sizeBytes> readDoc;
  deserializeJson(readDoc, content);

  Serial.println("Read eeprom document:");
  Serial.println(content);
  
  values.ssid = readDoc["wifi"]["ssid"].as<String>();
  values.password = readDoc["wifi"]["password"].as<String>();

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

  StaticJsonDocument<sizeBytes> writeDoc;
  writeDoc["wifi"]["ssid"] = config.ssid;
  writeDoc["wifi"]["password"] = config.password;
  
  writeDoc["mqtt"]["server"] = config.mqttServer;
  writeDoc["mqtt"]["username"] = config.mqttUsername;
  writeDoc["mqtt"]["password"] = config.mqttPassword;
  writeDoc["mqtt"]["port"] = config.mqttPort;
  writeDoc["mqtt"]["topic"] = config.mqttSensorTopic;

  Serial.print("WRITEEEPROM MQTT Payload:");
  Serial.println(config.mqttServer);
  Serial.println(config.mqttSensorTopic);
  Serial.println(config.mqttPort);

  String _json;
  serializeJson(writeDoc, _json);

  Serial.println("writing eeprom document:");
  Serial.println(_json);
  for (int i = 0; i < _json.length(); ++i)
  {
    EEPROM.write(i, _json[i]);
  }
  EEPROM.commit();
  delay(1000);
}




StorageValues EEPROM_setup() {
  EEPROM.begin(sizeBytes); //Initialasing EEPROM
  delay(10);

  if(false) {
    wipeEEPROM();    
  }
  return readEEPROMData();
}
