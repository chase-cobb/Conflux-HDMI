#!/bin/bash

DIR="$( cd "$( dirname "$0" )" && pwd )"

echo $DIR

#Project directories
FIRMWARE_UPDATE=$DIR/Firmware_Update
CONFIGURABLE_FEATURES=$DIR/Configurable_Features
SIMPLE_INFO=$DIR/Simple_Info

cd $FIRMWARE_UPDATE
make clean

cd $CONFIGURABLE_FEATURES
make clean

cd $SIMPLE_INFO
make clean
