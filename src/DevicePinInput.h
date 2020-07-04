#pragma once


//Includes---------------------------------------------------------------------
#include "AlmondPrecompiled.h"
#include "Device.h"
#include "TimerOverride.h"


//Types------------------------------------------------------------------------
class DevicePinInput: public DeviceInput
{
	public:
	static const int poll_interval = 100;

	DevicePinInput(const char *name,
				  uint8_t pin,
				  uint8_t filter_len,
				  bool invert);
	void setup() override;
	void loop() override;

protected:
	TimerOverride timer;
	int16_t filter_sum;
	uint8_t pin;
	uint8_t filter_len;
	bool invert;

};
