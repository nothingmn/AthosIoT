AthosIoT
=========

Objective
---------
Leveraging ESP8266 and Node-Red on a RasperryPi to build a sensor and relay network. 

Core goals
----------
A set of slave nodes leveraging ESP8266 devices (nodemcu, d1 mini, etc..) which will report sensor data and receive relay command and control data via MQTT.

A RaspberryPi will be available, with Node-Red installed in order to act as a Master node on the network.  It will be responsible for ensuring the slave nodes are properly configured, and that the data flows of sensor data and command and control data are done correctly.  Node-Red will also be responsible for for all user facing dashboards, etc..

All slave configuration will be stored on the slaves only.

All master configuration will be represented within Node-Red UI.

There will be no external dependancies, for example a database.

There will be no provision to store historical data on either any slave nor master nodes.  If you want to keep the data, wire up Node-Red to persist the data some place.  The reason why we dont want to install a database on the master node is three fold:

1. Keep the end solution simple with low to no dependancies
2. SD Cards in our master node, a RPi will have a very short life.  We opt for a longer life span
3. Node-red allows you to offload your MQTT data to any sort of database on your network.

Allow for multiple types of sensors, from a basic TMP sensor to a more advanced BMP sensor.  


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

