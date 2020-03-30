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
1. Support and tested on NodeMCUv1
2. Support for a TMP36 Sensor (Temperature only)
3. Support for DHT11 Sensor (Temperature, head index, and humidity)
4. Support for BMP280 Sensor (Temperature, humidity, air pressure, altitude)
5. If Wifi is not configured, it will setup a soft AP for the user to join, and configure Wifi 
6. Node will automatically find (on the network via UDP) the Node-Red instance, and the Node-Red instance will send a UDP configuration packet back to the Node to get the initial setup complete
7. Support for unlimited number of GPIO based Relays.  
8. All sensor report data to MQTT
9. Relay subscriibes to MQTT topic, and will respond by controlling the relays
10. We also have more global commands, reset, restart, wipe and reconfigure; which gives you braoder control over the device.  All of these changes will cause the device to recycle.
11. All nodes automatically generate a "deviceid", which is the last 6 digits of the device Mac Address which we determine is unique enough.  I uses this device ID as the Soft AP, the ClientID for MQTT and it adds it to each message we send to MQTT.
12. NTP Time server sync and constantly self-updating
13. Internal/Onboad LED control, with helper methods to wrap the LEDs
14. Save and Load all configuration from EEPROM, so they last across restarts.
15. Each node will self-report capabilities to MQTT
16. Node-red instance stores the deviceid and device name mappings
17. Node-red will take the raw sensor data, and perform basic transform to it to ensure it is ready for consumption downstreams, then re-queues


Possible Major Features
-----------------------
1. Remote flashing new firmware on the device (via MQTT/URL?)  (DONE)
2. PIR Sensors (DONE)
3. Remote configuration of sensor tolerances and minimum reporting timing
4. GPS Support
5. Partially connected scenarios
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

Slave node sequencing
--------------------
1. Read EEPROM for device configuration
2. Ensure Wifi is setup (from device config)
   1. If not configured setup a soft access point, and allow the user to specify the AP.
   2. If configured then connect to the wifi access point
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
*Install MQTT and Node-Red*
1. Install Rasbian on a Raspberry Pi  https://www.raspberrypi.org/documentation/installation/installing-images/
2. Once you have the OS installed, SSH into it
3. Install our MQTT Server (Mosquitto)   sudo apt-get install mosquitto
4. Optionally setup credentials for your Mosquitto installation http://www.steves-internet-guide.com/mqtt-username-password-example/
5. Install Node-Red   https://nodered.org/docs/getting-started/raspberrypi
6. Optionally setup credentials for your Node-Red instance https://nodered.org/docs/user-guide/runtime/securing-node-red
7. Launch your browser, and point your browser to  http://{raspberrypi}:1880, where {raspberrypi} is either your DNS name or IP address of your new raspberrypi machine. Login in if necessary
8. Fork this repository, and clone your fork to your development machine
9. In the node-red folder, there is a flows.json file.  Upload that via the "Import from clipboard" functionality in Node-Red.  Typically this is in the top right menu.
10. Update the configuration for the MQTT server, just under the hamburger menu on the top right, there is a down arrow, choose "Configuration Nodes" and on "On All Flows" section you should see a node for MQTT.  Change that to your local instance (its find to use localhost)
11. Open the "Form reponse packet" node on the "Flow 1" Tab, and change the "server" variable to the IP address of your MQTT machine.  This CANNOT be localhost or 127.0.0.1.  Give it the IP address of your MQTT instance, or some other DNS name.  This is the value which the slave nodes will use to resolve the MQTT server.
12. Assuming the flows were correctly imported, Deploy your node-red instance (top right "Deploy" button)

*Install VSCode and PlatformIO*

13. Install Visual Studio Code from here: https://code.visualstudio.com/Download
14. Install PlatformIO from within Visual Studio Code, be patient, it takes a while https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide
15. Open the file "Athos.code-workspace"
16. Plug in your NodeMCU (ESP8266 device) into your machine
17. You should be able to hit Control-Alt-B to build, Control-Alt-U to upload

