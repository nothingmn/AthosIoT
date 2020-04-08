AthosIoT
=========

Build Status
------------
[![Main Build](https://github.com/nothingmn/AthosIoT/workflows/Main%20Build/badge.svg)](https://github.com/nothingmn/AthosIoT/actions)

Objective
---------
Leveraging ESP8266 and Node-Red on a RasperryPi to build a sensor and relay network. 

Core goals
----------
A set of slave nodes leveraging ESP8266 devices (nodemcu, d1 mini, etc..) which will report sensor data and receive relay command and control data via MQTT.  (DONE)

A RaspberryPi will be available, with Node-Red installed in order to act as a Master node on the network.  It will be responsible for ensuring the slave nodes are properly configured, and that the data flows of sensor data and command and control data are done correctly.  Node-Red will also be responsible for for all user facing dashboards, etc..  (DONE)

All slave configuration will be stored on the slaves only.  (DONE)

All master configuration will be represented within Node-Red UI.  (DONE)

There will be no external dependancies, for example a database.  (DONE)

There will be no provision to store historical data on either any slave nor master nodes.  If you want to keep the data, wire up Node-Red to persist the data some place.  The reason why we dont want to install a database on the master node is three fold:

1. Keep the end solution simple with low to no dependancies
2. SD Cards in our master node, a RPi will have a very short life.  We opt for a longer life span
3. Node-red allows you to offload your MQTT data to any sort of database on your network.

Allow for multiple types of sensors, from a basic TMP sensor to a more advanced BMP sensor.    (DONE)

Major Features
--------------
1. Support and tested on NodeMCUv1  (DONE)
2. Support for a TMP36 Sensor (Temperature only)  (DONE)
3. Support for DHT11 Sensor (Temperature, head index, and humidity)  (DONE)
4. Support for BMP280 Sensor (Temperature, humidity, air pressure, altitude)  (DONE)
5. If Wifi is not configured, it will setup a soft AP for the user to join, and configure Wifi   (DONE)
6. Node will automatically find (on the network via UDP) the Node-Red instance, and the Node-Red instance will send a UDP configuration packet back to the Node to get the initial setup complete  (DONE)
7. Support for unlimited number of GPIO based Relays.    (DONE)
8. All sensor report data to MQTT  (DONE)
9. Relay subscriibes to MQTT topic, and will respond by controlling the relays  (DONE)
10. We also have more global commands, reset, restart, wipe and reconfigure; which gives you braoder control over the device.  All of these changes will cause the device to recycle.  (DONE)
11. All nodes automatically generate a "deviceid", which is the last 6 digits of the device Mac Address which we determine is unique enough.  I uses this device ID as the Soft AP, the ClientID for MQTT and it adds it to each message we send to MQTT.  (DONE)
12. NTP Time server sync and constantly self-updating  (DONE)
13. Internal/Onboad LED control, with helper methods to wrap the LEDs  (DONE)
14. Save and Load all configuration from EEPROM, so they last across restarts.  (DONE)
15. Each node will self-report capabilities to MQTT  (DONE)
16. Node-red instance stores the deviceid and device name mappings  (DONE)
17. Node-red will take the raw sensor data, and perform basic transform to it to ensure it is ready for consumption downstreams, then re-queues  (DONE)


Possible Major Features
-----------------------
1. Remote flashing new firmware on the device (via MQTT/URL?)  (DONE)
2. PIR Sensors (DONE)
3. Node-side sensor data smoothing, moving average and tolerance based reporting  (DONE)
4. GPS Support
5. Partially connected scenarios (DONE, via MQTT and WiFi will reconnect gracefully)
6. PH Sensor
7. Water (level) Sensor
8. Volumentric water content sensor
9. EC sensor
10. Soil water potential sensor
11. Light levels (Lux) sensor
12. CO2 sensor
13. Shade control
14. Dosing / Irrigation control
15. Lighting systems
16. Passive / Active cooling
17. Heating
18. Harvesting equipment
19. Spraying equipment
20. Door locks
21. Foggers / Humidifiers
22. Smart Phone App
23. Better web API and portal
24. Alerting (SMS, Telegram, PushBullet)

Slave node sequencing (DONE)
--------------------
1. Read EEPROM for device configuration
2. Ensure Wifi is setup (from device config)
   1. If not configured, attempt to connect to RaspberryPi's Hidden Access Point (which is isolated from users home network)
   2. Otherwise, if configured then connect to the wifi access point
   3. Setup an NTP client, and ensure that the NTP client is up to date (self updates, every XX minutes)
   4. Ensure MQTT is configured
5. If not configured, broadcast a UDP packet (255.255.255.255:3000)
   1. This UDP Package will be picked up by Node-Red and will send a configuration message to the specific device via UDP directly.
   2. The device will receive the UDP package, and store the configuration
6. If configured then connect to the MQTT server specified
7. Setup the specific sensor devices and enable independant looping of of gathering of sensor data
8. Setup subscriptions for relays for MQTT

Getting started
-----------------------
1. For the rasperrypi machine, download our Pi4/16GB SD Card Image here
2. Use a tool similar to win32diskimager to write the image to your 16GB card
3. Boot. 


*Install VSCode and PlatformIO*

1. Install Visual Studio Code from here: https://code.visualstudio.com/Download
2. Install PlatformIO from within Visual Studio Code, be patient, it takes a while https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide
3. Open the file "Athos.code-workspace"
4. Plug in your NodeMCU (ESP8266 device) into your machine
5. You should be able to hit Control-Shift-B and choose which node-type to build and/or upload

Once you have the raspberrypi machine online browse to the admin page at http://raspberrypi, or use the IP address as per your route. At a minimum update the "Configure Node Wifi" page with the desired Wifi AP.  Next, the ESP8266 nodes should automatically discover and connect once they are powered up; and once they do the "Managed Devices" screen should start to pupulate.
