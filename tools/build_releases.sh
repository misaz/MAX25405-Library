#!/bin/bash

ORIGINAL_PWD=$(pwd)

mkdir -p releases

# Create generic variant
TEMP=$(mktemp)
rm $TEMP
mkdir $TEMP
cp src/MAX25405.h $TEMP/MAX25405.h
cp src/MAX25405.c $TEMP/MAX25405.c
cp src/MAX25405_PlatformSpecific.h $TEMP/MAX25405_PlatformSpecific.h
cp src/MAX25405_PlatformSpecific.c $TEMP/MAX25405_PlatformSpecific.c
cd $TEMP
zip MAX25405_Generic.zip *
cd $ORIGINAL_PWD
cp $TEMP/MAX25405_Generic.zip releases/
rm -r $TEMP

# Create MAX78000 variant
TEMP=$(mktemp)
rm $TEMP
mkdir $TEMP
cp src/MAX25405.h $TEMP/MAX25405.h
cp src/MAX25405.c $TEMP/MAX25405.c
cp src/MAX25405_PlatformSpecific_MAX78000.h $TEMP/MAX25405_PlatformSpecific.h
cp src/MAX25405_PlatformSpecific_MAX78000.c $TEMP/MAX25405_PlatformSpecific.c
cd $TEMP
zip MAX25405_MAX78000.zip *
cd $ORIGINAL_PWD
cp $TEMP/MAX25405_MAX78000.zip releases/
rm -r $TEMP
