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
DHT22SOURCE=$BUILDROOT/DHT22/$BINFILE
PIRSOURCE=$BUILDROOT/PIR/$BINFILE
MOISTSOURCE=$BUILDROOT/MOIST/$BINFILE
MOIST_BMP280SOURCE=$BUILDROOT/MOIST_BMP280/$BINFILE
MQ2SOURCE=$BUILDROOT/MQ2/$BINFILE
RELAY_BMP280_MQ2SOURCE=$BUILDROOT/RELAY_BMP280_MQ2/$BINFILE
RELAY_MQ2SOURCE=$BUILDROOT/RELAY_MQ2/$BINFILE
STUBSOURCE=$BUILDROOT/STUB/$BINFILE
MQ135SOURCE=$BUILDROOT/MQ135/$BINFILE
MQ9SOURCE=$BUILDROOT/MQ9/$BINFILE
MQ8SOURCE=$BUILDROOT/MQ8/$BINFILE
MQ7SOURCE=$BUILDROOT/MQ7/$BINFILE
MQ6SOURCE=$BUILDROOT/MQ6/$BINFILE
MQ5SOURCE=$BUILDROOT/MQ5/$BINFILE
MQ4SOURCE=$BUILDROOT/MQ4/$BINFILE
MQ3SOURCE=$BUILDROOT/MQ3/$BINFILE
BUTTONSOURCE=$BUILDROOT/BUTTON/$BINFILE

ATH_RELAY_MD0DEST=$OUTROOT/ATH_RELAY_MD0_$BINFILE
ATH_RELAY_PIRDEST=$OUTROOT/RELAY_PIR_$BINFILE
RELAYDEST=$OUTROOT/RELAY_$BINFILE
TMP36DEST=$OUTROOT/TMP36_$BINFILE
BMP280DEST=$OUTROOT/BMP280_$BINFILE
DHT11DEST=$OUTROOT/DHT11_$BINFILE
DHT22DEST=$OUTROOT/DHT22_$BINFILE
PIRDEST=$OUTROOT/PIR_$BINFILE
MOISTDEST=$OUTROOT/MOIST_$BINFILE
MOIST_BMP280DEST=$OUTROOT/MOIST_BMP280_$BINFILE
MQ2DEST=$OUTROOT/MQ2_$BINFILE
RELAY_BMP280_MQ2DEST=$OUTROOT/RELAY_BMP280_MQ2_$BINFILE
RELAY_MQ2DEST=$OUTROOT/RELAY_MQ2_$BINFILE
STUBDEST=$OUTROOT/STUB_$BINFILE
MQ135DEST=$OUTROOT/MQ135_$BINFILE
MQ9DEST=$OUTROOT/MQ9_$BINFILE
MQ8DEST=$OUTROOT/MQ8_$BINFILE
MQ7DEST=$OUTROOT/MQ7_$BINFILE
MQ6DEST=$OUTROOT/MQ6_$BINFILE
MQ5DEST=$OUTROOT/MQ5_$BINFILE
MQ4DEST=$OUTROOT/MQ4_$BINFILE
MQ3DEST=$OUTROOT/MQ3_$BINFILE
BMP280_MQ2DEST=$OUTROOT/BMP280_MQ2_$BINFILE
BUTTONDEST=$OUTROOT/BUTTON_$BINFILE

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
cp $DHT22SOURCE $DHT22DEST
cp $PIRSOURCE $PIRDEST
cp $MOISTSOURCE $MOISTDEST
cp $MOIST_BMP280SOURCE $MOIST_BMP280DEST
cp $MQ2SOURCE $MQ2DEST
cp $RELAY_BMP280_MQ2SOURCE $RELAY_BMP280_MQ2DEST
cp $RELAY_MQ2SOURCE $RELAY_MQ2DEST
cp $STUBSOURCE $STUBDEST
cp $MQ135SOURCE $MQ135DEST
cp $MQ9SOURCE $MQ9DEST
cp $MQ8SOURCE $MQ8DEST
cp $MQ7SOURCE $MQ7DEST
cp $MQ6SOURCE $MQ6DEST
cp $MQ5SOURCE $MQ5DEST
cp $MQ4SOURCE $MQ4DEST
cp $MQ3SOURCE $MQ3DEST
cp $BMP280_MQ2SOURCE $BMP280_MQ2DEST
cp $BUTTONSOURCE $BUTTONDEST