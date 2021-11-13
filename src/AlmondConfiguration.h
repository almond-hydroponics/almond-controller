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
	static int get_device_id() ;

protected:
	static void setup_i2c();
	static void setup_fs();
	static void setup_led_outputs();
};

extern AlmondConfiguration ALMOND_CONFIGURATION;
