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
	void setup();
	void loop();

	unsigned int get_free_heap();

protected:
	static void setup_i2c();
	void setup_fs();
};

extern AlmondConfiguration ALMOND_CONFIGURATION;
