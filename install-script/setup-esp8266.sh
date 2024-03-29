#!/bin/bash

echo "**** Dev Environment Setup Started ****"

printf "\n"
#**Save Original Path - will need this when switching between versions**
printf "Create base path variable\n"
export SavePath=$PATH
printf "SavePath Env variable set to original Path:\n"
echo "$SavePath"
printf "\n"

##**Set up my Code Path**
mkdir -p ~/Documents/Projects/Esp
export MyCodePath=~/Documents/Projects/Esp
printf "\n"
printf "\nCode path: $MyCodePath\n"
cd $MyCodePath || exit

# **Create ESP directory**
printf "Create ESP SDK and toolchain directory\n"
mkdir -p esp
cd esp || exit

#**Set up ESP8266 Toolchain**
printf "Get ESP8266 64 bit Toolchain\n"
wget https://dl.espressif.com/dl/xtensa-lx106-elf-gcc8_4_0-esp-2020r3-macos.tar.gz
printf "ESP8266 64 bit stable Toolchain download completed\n"
printf "\n"

#**Get ESP8266 Stable SDK/APIs**
printf "\nInstall Stable version of ESP9266 Toolchain & SDK v3.3\n"
cd $MyCodePath/esp || exit
mkdir -p 8266
cd 8266 || exit
printf "Extract 64 bit ESP8266 Toolchain to Folder\n"
tar -xzf "$MyCodePath/esp/xtensa-lx106-elf-gcc8_4_0-esp-2020r3-macos.tar.gz"
printf "64 bit ESP32 Toolchain (stable) Extraction Complete\n"
printf "Get version of ESP8266 SDK \n"
git clone --recursive https://github.com/espressif/ESP8266_RTOS_SDK.git
printf "Latest version of ESP8266 SDK Completed\n\n"

#**Set ESP8266 path for stable toolchain and ESP tools by default"
export ESP8266_PATH=$MyCodePath/esp/8266
export IDF_PATH=$MyCodePath/esp/8266/ESP8266_RTOS_SDK
printf "IDF_PATH is: $IDF_PATH\n"
export IDF_TOOLS_PATH=$ESP8266_PATH/ESP8266_RTOS_SDK/tools
echo "IDF tools path: $IDF_TOOLS_PATH\n"
export IDF_8266_PATH=$ESP8266_PATH/ESP8266_RTOS_SDK
echo "8266 latest IDF_8266_PATH: $IDF_8266_PATH\n"

#**Setup PATH for ESP toolchain, SDK, Tools
printf "Original Path is:\n $PATH\n\n"
printf "Add ESP8266 toolchain, SDK, and Tools to Path\n"
export PATH="$ESP8266_PATH/xtensa-lx106-elf/bin:$IDF_TOOLS_PATH:$PATH"
printf "Updated path with ESP IDF and Toolchain:\n $PATH\n\n"
echo "Install/Update python requirements for ESP Stable IDF & SDK\n"
/usr/bin/python -m pip install --user -r $IDF_PATH/requirements.txt
printf "**** Dev Environment Setup Completed ****\n"
