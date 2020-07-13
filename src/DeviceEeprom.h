#pragma once

//Includes---------------------------------------------------------------------
#include "AlmondPrecompiled.h"
#include "AlmondConfiguration.h"

#define EEPROM_END		0
#define EEPROM_START	1


//Types------------------------------------------------------------------------
void readCredentials();
bool needEraseEEPROM();
void clearParam();
void writeEEPROM(int address, char *data, int size);
int readEEPROM(int address, char *buff);
