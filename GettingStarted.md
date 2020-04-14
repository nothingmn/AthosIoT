# AthosIoT – Technical Brief

## Document Overview

This document is intended to outline the various software and hardware components of the AthosIoT system.  We will start with an overview of the architecture and then dive into the major components and their details one by one.

**Source Control Details**

1. [Code Of Conduct]("https://github.com/nothingmn/AthosIoT/blob/master/CODE_OF_CONDUCT.md")
1. [Contributing](https://github.com/nothingmn/AthosIoT/blob/master/CONTRIBUTING.md)
1. [License](https://github.com/nothingmn/AthosIoT/blob/master/LICENSE)
1. [Bug reports, feature requests](https://github.com/nothingmn/AthosIoT/issues/new/choose)
1. [Build status](https://github.com/nothingmn/AthosIoT/actions?query=workflow%3A%22Main+Build%22)

----

## Table of Contents
1. [General Architecture](#general-architecture)
   1.1 [Overview](#overview)
   1.2 [Target Audience](#target-audience)

2. [AthosIoT Hub](#athosiot-hub)
2.1.	[RaspberryPi 4](#raspberrypi-4)
2.1.1.		[Primary purpose and usage](#primary-purpose-and-usage")
2.1.2.		[Getting setup](#getting-setup)
2.1.2.1.			[Download Image, 16GB card needed](#download-image-16gb-card-needed)
2.1.2.2.			[Tools for formatting the SD Card](#tools-for-formatting-the-sd-card)
2.1.2.3.			[Tools for writing the image onto the DS Card](#tools-for-writing-the-image-onto-the-ds-card)
2.1.3.		[File locations:](#file-locations)
2.1.3.1.			[Ssh-keys for github (AthosIoT.Web)](#ssh-keys-for-github-athosiotweb)
2.1.3.2.			[Authorized keys file](#authorized-keys-file)
2.1.3.3.			[Athos.service file](#athosservice-file)
2.1.3.4.			[Node-Red settings, temp device storage and global context files](#node-red-settings-temp-device-storage-and-global-context-files)
2.1.3.5.			[Node-red service file](#node-red-service-file)
2.1.3.6.			[RaspAP config file](#raspap-config-file)
2.1.3.7.			[MQTT Configuration](#mqtt-configuration)

3.	[Node-Red](#node-red)
3.1.		[Why Node-Red vs build your own](#why-node-red-vs-build-your-own)
3.1.1.           [Basic flows](#basic-flows)
3.2.		[How to add backup to your instance](#how-to-add-backup-to-your-instance)
3.2.1.			[Recommend a cron + shell script](#recommend-a-cron-shell-script)
3.3.		[How to change the username/password](#how-to-change-the-usernamepassword)
3.4.		[Adding your own flows](#adding-your-own-flows)
3.4.1.			[Your own Node-Red instance, and MQTT](#your-own-node-red-instance-and-mqtt)
3.4.2.			[Useful flows](#useful-flows)

4.	[MQTT](#mqtt)
4.1.		[Why are we using it, what function does it perform?](#why-are-we-using-it-what-function-does-it-perform)
4.2.		[How to change the username/password](#how-to-change-the-usernamepassword-1)
4.3.		[List of topics, what they are for](#list-of-topics-what-they-are-for)

5.	[RaspAP](#raspap)
5.1.		[Why are we using it, what function does it perform?](#why-are-we-using-it-what-function-does-it-perform-1)
5.2.		[How to change the username/password](#how-to-change-the-usernamepassword-2)

6.	[AthosIoT.Web](#athosiotweb)
6.1.		[WebSocket, and tie in with node-red](#websocket-and-tie-in-with-node-red)
6.2.		[An overview of the user interface](#an-overview-of-the-user-interface)

7.	[Enclosures](#enclosures)
7.1		[Raspberry PI](#raspberry-pi)
7.2		[NodeMCU](#nodemcu)

8.	[OTA process](#ota-process)
8.1.	[How to trigger the OTA update](#how-to-trigger-the-ota-update)
8.2		[Web Server](#web-server)

9. [AthosIoT Nodes](#athosiot-nodes)
9.1.    [General design](#general-design)
9.2.        [Boot process](#boot-process)
9.3.        [Getting VSCode setup correctly](#getting-vscode-setup-correctly)
9.4.        [Using tasks to target builds](#using-tasks-to-target-builds)
9.5.        [Hardware](#hardware)
9.5.1.            [ESP8266 and Variants](#esp8266-and-variants)
9.5.2.                [NodeMCU](#nodemcu-1)
9.5.3.                [Wemos D1](#wemos-d1)
9.6.            [Parts List of Sensors / Actuator](#parts-list-of-sensors-actuator)
9.6.1.                [How to connect each device](#how-to-connect-each-device)
9.6.2.                [OTA process](#ota-process-1)
9.7.    	[Global Actions](#global-actions)
9.7.1.	    	[Reset, Wipe, Ping, etc](#reset-wipe-ping-etc)
9.7.2.	    [Relays, pin mappings and the UI](#relays-pin-mappings-and-the-ui)

8. [GitHub Actions](#github-actions)
8.1 [How to change the build](#how-to-change-the-build)

9. [Build.sh](#buildsh)
9.1.    [General design](#general-design-1)
9.2.    [What is it used for?](#what-is-it-used-for)

----

## General Architecture

### Overview
The AthosIoT project was designed for small to medium sized project installations, with no explicit limitations on the number of nodes you can add to your network.  Generally speaking all when we refer to “Hub” or “Master” it means the Raspberry PI where “Node” or “Slave” refers to any of the ESP8266 devices on the network.  

One of the bigger goals for the Hub was to leverage as much open source projects as we could in order to simplify the solution and management of.  We wanted to be able to leverage those projects existing documentation and ecosystem in order to jump-start the progress of this project.  For example, instead of writing our own logic to implement a Hidden Access Point in the Hub, we leverage RaspAP and have it turned on by default. (We will explain this in more detail below).  The point is, is that by leveraging all the hard work by the RaspAP team, after a basic setup and configuration of that tool, we can simply mark that task as done with and move on to more important details.  

We chose a Raspberry Pi 4 for our hub simply because it was the most powerful Raspberry Pi 4 on the market at the time of this projects creation.  It brings more then enough CPU power and Network bandwidth for our needs.  Not to mention the community and ecosystem is huge and well cared for.  Kudos the the Raspberry Pi team and communities for such a great job.

Our Hub is the brains of the entire project, and within that, we rely heavily on the Node-Red open source project.  This means with very little knowledge of JavaScript the average consumer should be able to manage their own installation with ease.  That ecosystem is also flourishing and mature enough to be depended upon.  We will get into more details about our flows within node-red further on.

Our basic goal for our Nodes were one of simplicity.  That is, we try to keep any and all logic off of those devices as much as possible.  The less code we have the easier they will be to manage, keep up to date and grow.  We understand the difficulty in the sheer variety of options people will want for their nodes (different sensors, actuators, etc..) and instead of creating a configuration and deployment system we have opted for a much smaller and controlled set of Nodes + Sensors/Actuators and a set of pre-defined builds.  If you need more variety, feel free to enhance the Build.sh, vscode tasks.json, platform.ini, and github actions in the form of a pull request.

The final piece to mention here is that we went out of our way to automate the setup and configuration of our nodes, details are provided in the AthosIoT Nodes section, but the high-level overview of that process is this.  Our goal was to not have any human interaction with our Nodes in order to get them setup and on the network.  Essentially plug and play.  We facilitate this by having an hidden Access Point on the Hub which is well known (and hard coded) to the Nodes.  If the Nodes fail to connect to their configured AP (or it has not been configured yet) they will attempt to connect to this hidden network.  Once connected they will attempt to retrieve the configuration either by HTTP or UDP.  Once configured, they reset and during the boot they connect to the desired AP, MQTT and so on.  This allows for a very fault tolerant network which the administrator can feel free to OTA (Over The Air update) and / or wipe device’s memory with little to no repercussions.  They will simply follow that bootstrapping process again and reconnect.  The only real way to take one of these nodes out of commission will be to unplug it.  Uptime is very important.

### Target Audience
As you can tell we tried to make the installation and management of this entire project very easy, with UI’s where appropriate.  This makes the project very consumable by non-hard-core engineers.  You don’t have to have a degree in electronics engineering or even be a seasoned developer to install, setup, grow and manage your own IoT system.

## AthosIoT Hub
###	RaspberryPi 4
####		Primary purpose and usage
####		Getting setup
#####			Download Image, 16GB card needed
#####			Tools for formatting the SD Card
#####			Tools for writing the image onto the DS Card
####		File locations:
#####			Ssh-keys for github (AthosIoT.Web)
#####			Authorized keys file
#####			Athos.service file
#####			Node-Red settings, temp device storage and global context files
#####			Node-red service file
#####			RaspAP config file
#####			MQTT Configuration

###	Node-Red
####		Why Node-Red vs build your own
#####           Basic flows
####		How to add backup to your instance
#####			Recommend a cron + shell script
####		How to change the username/password
####		Adding your own flows
####			Your own Node-Red instance, and MQTT
####			Useful flows
###	MQTT
####		Why are we using it, what function does it perform?
####		How to change the username/password
####		List of topics, what they are for
###	RaspAP
####		Why are we using it, what function does it perform?
####		How to change the username/password
###	AthosIoT.Web
####		WebSocket, and tie in with node-red
####		An overview of the user interface
###	Enclosures
####		Raspberry PI
####		NodeMCU
###	OTA process
####		How to trigger the OTA update
####		Web Server
### AthosIoT Nodes
####    General design
####        Boot process
####        Getting VSCode setup correctly
####        Using tasks to target builds
####        Hardware
#####            ESP8266 and Variants
#####                NodeMCU
#####                Wemos D1
####            Parts List of Sensors / Actuator
#####                How to connect each device
#####                OTA process	
####    	Global Actions
#####	    	Reset, Wipe, Ping, etc
####	    Relays, pin mappings and the UI
### GitHub Actions
#### How to change the build
### Build.sh
#####    General design
#####    What is it used for?


	
