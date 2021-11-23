#pragma once

// Includes---------------------------------------------------------------------
#include "AlmondPrecompiled.h"
#include "Device.h"
#include "TimerOverride.h"

// Types------------------------------------------------------------------------
class WaterLevel : public DeviceInput
{
	public:
	static const int nfail_limit = 16;
	static const int measure_interval = 10000;

	WaterLevel(const char* name, uint8_t pin_trigger, uint8_t pin_echo);
	void setup() override;
	void loop() override;

	protected:
	void measure_pulse_in();

	TimerOverride timer;
	uint8_t pin_trigger;
	uint8_t pin_echo;
	uint8_t nfails;
};
