#!/bin/bash
PLATFORMIOBIN=/mnt/c/Users/robch/.platformio/penv/Scripts/platformio.exe
#PLATFORMIOBIN=C:/Users/robch/.platformio/penv/Scripts/platformio.exe
BUILDROOT=./.pio/build
OUTROOT=./out/

if [ ! -d $OUTROOT ]; then
    echo $OUTROOT
    mkdir -p $OUTROOT;
else
    rm $OUTROOT/* -f
fi

BINFILE=firmware.bin
ATH_RELAY_MD0SOURCE=$BUILDROOT/ATH_RELAY_MD0/$BINFILE
ATH_RELAY_PIRSOURCE=$BUILDROOT/RELAY_PIR/$BINFILE
RELAYSOURCE=$BUILDROOT/RELAY/$BINFILE
TMP36SOURCE=$BUILDROOT/TMP36/$BINFILE
BMP280SOURCE=$BUILDROOT/BMP280/$BINFILE
DHT11SOURCE=$BUILDROOT/DHT11/$BINFILE
PIRSOURCE=$BUILDROOT/PIR/$BINFILE
MOISTSOURCE=$BUILDROOT/MOIST/$BINFILE
MOIST_BMP280SOURCE=$BUILDROOT/MOIST_BMP280/$BINFILE

ATH_RELAY_MD0DEST=$OUTROOT/ATH_RELAY_MD0_$BINFILE
ATH_RELAY_PIRDEST=$OUTROOT/RELAY_PIR_$BINFILE
RELAYDEST=$OUTROOT/RELAY_$BINFILE
TMP36DEST=$OUTROOT/TMP36_$BINFILE
BMP280DEST=$OUTROOT/BMP280_$BINFILE
DHT11DEST=$OUTROOT/DHT11_$BINFILE
PIRDEST=$OUTROOT/PIR_$BINFILE
MOISTDEST=$OUTROOT/MOIST_$BINFILE
MOIST_BMP280DEST=$OUTROOT/MOIST_BMP280_$BINFILE

echo -e "-----------CLEANING-----------" 
$PLATFORMIOBIN  run --target clean 
echo -e "-----------DONE CLEANING-----------" 
echo -e "-----------BUILDING-----------" 
$PLATFORMIOBIN run
$PLATFORMIOBIN run
echo -e "-----------DONE BUILDING-----------" 
cp $ATH_RELAY_MD0SOURCE $ATH_RELAY_MD0DEST
cp $ATH_RELAY_PIRSOURCE $ATH_RELAY_PIRDEST
cp $RELAYSOURCE $RELAYDEST
cp $TMP36SOURCE $TMP36DEST 
cp $BMP280SOURCE $BMP280DEST
cp $DHT11SOURCE $DHT11DEST
cp $PIRSOURCE $PIRDEST
cp $MOISTSOURCE $MOISTDEST
cp $MOIST_BMP280SOURCE $MOIST_BMP280DEST