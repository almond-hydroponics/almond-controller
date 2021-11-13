#pragma once

//Includes---------------------------------------------------------------------
#include "AlmondPrecompiled.h"
#include "Device.h"
#include "TimerOverride.h"
#include "../lib/sht-sensor-lib/sht21.h"


//Implementation---------------------------------------------------------------
class EnvironmentSht: public DeviceInput
{
public:
	const int measure_interval = 10000;
	explicit EnvironmentSht(const char *name);
	~EnvironmentSht() override = default;
	void setup() override;
	void loop() override;

protected:
	TimerOverride timer;
	SHT21 sht;
};
