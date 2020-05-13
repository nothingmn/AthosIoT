//AthosDeviceData.h

#ifndef ATH_DEVICEDATA_
#define ATH_DEVICEDATA_

#include "AthosHelpers.h"
#include "AthosNTP.h"
//#include <ESP.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <Thread.h>

#ifndef AnalogSmooth_h
#include "AnalogSmooth.h"
#include "AnalogSmooth.cpp"
#endif

Thread deviceData_thread = Thread();


PubSubClient _devicedata_mqtt_client;
String _devicedata_deviceId;
StorageValues _devicedata_config;

AnalogSmooth RSSI_AnalogSmooth = AnalogSmooth(100);
float RSSI_LAST = 0;
float RSSI_TOLERANCE = 1;

bool DD_FirstBoot = true;

void sendDeviceDataToMQTT(float rssi)
{
    if(DD_FirstBoot) {
        DD_FirstBoot = false;
        long ts = NTP_getEpochTime();
        String csv = String(
            "UP," +
            getVersion() + "," +
            ts);

        const char *payload = csv.c_str();
        const char *topic = _devicedata_config.mqttSensorTopic.c_str();
        Log.trace("UP: Topic:%s\nPayload:%s\nLength:%i\n", topic, payload, csv.length());

        if (!_devicedata_mqtt_client.publish(topic, payload))
        {
            Log.trace("UP Data to MQTT Failed. Packet > 128?");
        }
        MQTTTransmitLed();
        delay(500);
    }

    long ts = NTP_getEpochTime();
    String csv = String(
        "DDD," +
        getVersion() + "," +
        ts + "," +
        ESP.getChipId() + "," +
        ESP.getCpuFreqMHz() + "," +
        ESP.getFlashChipVendorId() + "," +
        ESP.getFreeSketchSpace() + "," +
        ESP.getSketchMD5() + "," +
        ESP.getVcc());

    const char *payload = csv.c_str();
    const char *topic = _devicedata_config.mqttSensorTopic.c_str();
    Log.trace("DDD: Topic:%s\nPayload:%s\nLength:%i\n", topic, payload, csv.length());

    if (!_devicedata_mqtt_client.publish(topic, payload))
    {
        Log.trace("DDD Data to MQTT Failed. Packet > 128?");
    }
    MQTTTransmitLed();
    delay(500);

   String csv2 = String(
        "DDN," +
        getVersion() + "," +
        ts + "," +
        rssi + "," +
        WiFi.localIP().toString() + "," +
        WiFi.macAddress() + "," +
        WiFi.gatewayIP().toString() + "," +
        WiFi.SSID());


    const char *payload2 = csv2.c_str();
    const char *topic2 = _devicedata_config.mqttSensorTopic.c_str();
    Log.trace("DDN: Topic:%s\nPayload:%s\nLength:%i\n", topic2, payload2, csv2.length());

    if (!_devicedata_mqtt_client.publish(topic2, payload2))
    {
        Log.trace("DDN Data to MQTT Failed. Packet > 128?");
    }
    MQTTTransmitLed();
    delay(500);


   String csv3 = String(
        "DDV," +
        getVersion() + "," +
        ts + "," +
        ESP.getCoreVersion() + "," +
        ESP.getSdkVersion());

    const char *payload3 = csv3.c_str();
    const char *topic3 = _devicedata_config.mqttSensorTopic.c_str();
    Log.trace("DDV: Topic:%s\nPayload:%s\nLength:%i\n", topic3, payload3, csv3.length());

    if (!_devicedata_mqtt_client.publish(topic3, payload3))
    {
        Log.trace("DDV Data to MQTT Failed. Packet > 128?");
    }
    MQTTTransmitLed();
    delay(500);
}
void DeviceData_Loop() {
    //nothing to see here
    if(deviceData_thread.shouldRun()) {
		deviceData_thread.run();
    }
}
const int DD_max_diff = 1 * 60 * 60;
int DD_last = 0;

void DeviceData_CallBack()
{
    bool sendUpdate = false;
    int current = NTP_getEpochTime();
    int timeDiff = abs(current - DD_last);

    Log.trace("DeviceData_CallBack %i", timeDiff);

    if(timeDiff > DD_max_diff) {
        Log.trace("will update DD data based on NTP duration");
        DD_last = current;
        sendUpdate = true;
    }

    float rssi = RSSI_AnalogSmooth.smooth(WiFi.RSSI());

    float diff = abs(rssi - RSSI_LAST);
    

    if (abs(diff) > RSSI_TOLERANCE)
    {
        Log.trace("will update DD data based on RSSI Tolerance");
        sendUpdate = true;
        RSSI_LAST = rssi;
    }
    if(sendUpdate) {
        sendDeviceDataToMQTT(rssi);
    }
}


void DeviceData_Setup(PubSubClient mqtt_client, String deviceId, StorageValues rootConfig, int loop_delay)
{
    _devicedata_mqtt_client = mqtt_client;
    _devicedata_deviceId = deviceId;
    _devicedata_config = rootConfig;

    Serial.println("Starting up deviceData_thread...");
    
    // Configure myThread
	deviceData_thread.onRun(DeviceData_CallBack);
	deviceData_thread.setInterval(5000); //every 10 seconds

}

#endif