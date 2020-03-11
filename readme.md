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
# Read EEPROM for device configuration
# Ensure Wifi is setup (from device config)
## If not configured setup a soft access point, and allow the user to specify the AP.
## If configured then connect to the wifi access point
# Setup an NTP client, and ensure that the NTP client is up to date (self updates, every XX minutes)
# Ensure MQTT is configured
# If not configured, broadcast a UDP packet (255.255.255.255:3000)
## This UDP Package will be picked up by Node-Red and will send a configuration message to the specific device via UDP directly.
## The device will receive the UDP package, and store the configuration
# If configured then connect to the MQTT server specified
# Setup the specific sensor devices and enable independant looping of of gathering of sensor data
# Setup subscriptions for relays for MQTT
