AthosIoT
=========

Objective
---------
Leveraging ESP8266 and Node-Red on a RasperryPi to build a sensor and relay network. 

Core goals
----------
A set of slave nodes leveraging ESP8266 devices (nodemcu, d1 mini, etc..) which will report sensor data and receive relay command and control data via MQTT.

A RaspberryPi will be available, with Node-Red installed in order to act as a Master node on the network.  It will be responsible for ensuring the slave nodes are properly configured, and that the data flows of sensor data and command and control data are done correctly.  Node-Red will also be responsible for for all user facing dashboards, etc..

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
