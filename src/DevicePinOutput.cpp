//Includes---------------------------------------------------------------------
#include "AlmondPrecompiled.h"
#include "DevicePinOutput.h"


//Implementation---------------------------------------------------------------
DevicePinOutput::DevicePinOutput(
	const char *name,
	uint8_t pin,
	bool invert
)
	:
	DeviceOutput(name)
{
	this->pin = pin;
	this->invert = invert;
}

void DevicePinOutput::setup()
{
	pinMode(this->pin, OUTPUT);
	set_value(this->value);
}

void DevicePinOutput::set_value(int value, bool verbose)
{
	bool write_value = !invert ? (value != 0) : (value == 0);
	digitalWrite(this->pin, write_value);

	if (this->value != value && verbose) {
		printf("Device '%s' status now: %d", this->name, value);
	}
	this->value = value;
}
