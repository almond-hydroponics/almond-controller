#pragma once

//Includes---------------------------------------------------------------------
#include "AlmondPrecompiled.h"
#include "ApplicationConstants.h"
#include "Logger.h"
#include "TimerOverride.h"

//Types------------------------------------------------------------------------
class AlmondConfiguration
{
public:
	static void setup();
	void loop();

	static unsigned int get_free_heap();

protected:
	static void setup_i2c();
	static void setup_fs();
};

extern AlmondConfiguration ALMOND_CONFIGURATION;
