//AthosDHT11.cpp
 
#ifndef AthosDHT11_h
#define AthosDHT11_h
#include "AthosHelpers.h"
#include "AthosNTP.h"
#include "Arduino.h"
#include "DHT.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

class AthosDHT11
{
	public:
                AthosDHT11();
                AthosDHT11(AthosHelpers helpers, AthosNTP ntp);
                void DHT11_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay);
                void DHT11_Loop();        
	private:
                void DHT11_sendReadingToMQTT(float temp, float humidity, float heatIndex);
                void checkAndReportReadings();
                int _DHT11_loop_delay;
                PubSubClient _DHT11_mqtt_client;
                String _DHT11_deviceId;
                StorageValues _DHT11_config;
                AthosHelpers _DHT11_helpers;
                AthosNTP _DHT11_ntp;
};
#endif
