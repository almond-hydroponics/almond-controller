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

	bool connected();
	unsigned int get_free_heap();

protected:
	bool wifi_ok;
	TimerOverride wifi_lost_timer;

	void setup_wifi();
	static void setup_i2c();

	void loop_wifi();
};

extern AlmondConfiguration ALMOND_CONFIGURATION;
