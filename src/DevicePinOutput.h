#pragma once

// Includes---------------------------------------------------------------------
#include "AlmondPrecompiled.h"
#include "Device.h"

// Types------------------------------------------------------------------------
class DevicePinOutput : public DeviceOutput
{
	public:
	DevicePinOutput(const char* name, uint8_t pin, bool invert);
	void setup() override;
	void set_value(int value, bool verbose = false) override;

	protected:
	uint8_t pin;
	bool invert;
};
