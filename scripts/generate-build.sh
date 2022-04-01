#!/bin/bash

BUILD_TYPE=release
APP_FILE=SimpleViewApp.js

helpFunction()
{
   echo ""
   echo "Usage: $0  <options>"
   echo "Options:"
   echo "   -h      : help"
   echo "   -d      : debug build"
   echo "   -o      : output folder name"
   echo "   -a      : app file (path from packages/react-native-skia)"
   echo "   -p name : package output files with given name"
   exit 1 # Exit script after printing help
}

while getopts "hdp:o:a:" opt
do
   case "$opt" in 
	p) PACKAGE_NAME="$OPTARG" ;;
	d) BUILD_TYPE=debug ;;
	o) OUTPUT_FOLDER="$OPTARG" ;;
	a) APP_FILE="$OPTARG" ;;
	h) helpFunction ;;
   esac
done

if [ ! $OUTPUT_FOLDER ];
then
  if [[ "$BUILD_TYPE" == "debug" ]];
  then
      OUTPUT_FOLDER=out/Debug
  else
      OUTPUT_FOLDER=out/Release	
  fi	  
fi

if [[ "$BUILD_TYPE" == "debug" ]];
then 
   echo "Build type : Debug"
   gn gen --args='use_sysroot=false enable_vulkan=false icu_use_data_file=false skia_use_system_nopoll=true gl_use_glx=false' $OUTPUT_FOLDER
else
   echo "Build type : Release"
   gn gen --args='use_sysroot=false enable_vulkan=false icu_use_data_file=false skia_use_system_nopoll=true gl_use_glx=false is_debug=false is_component_build=true' $OUTPUT_FOLDER
fi

yarn
ninja -C $OUTPUT_FOLDER
yarn workspace react-native-skia run react-native bundle \
    --platform ios \
    --dev false \
    --entry-file $APP_FILE \
    --bundle-output ../../$OUTPUT_FOLDER/SimpleViewApp.bundle \
    --assets-dest ../../$OUTPUT_FOLDER

if [ -n $PACKAGE_NAME ];
then
   cd $OUTPUT_FOLDER
   tar -cf $PACKAGE_NAME.tar ReactSkiaApp *.so *.bundle assets
   cd -   
fi
