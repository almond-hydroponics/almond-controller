//Includes---------------------------------------------------------------------
#include "Device.h"
#include "AlmondPrecompiled.h"


//Implementation---------------------------------------------------------------
Device::Device(const char *name)
{
	this->name = name;
	this->value = 0;
}

void Device::loop()
{
}

void Device::setup()
{
}

DeviceInput::DeviceInput(const char *name)
	: Device(name)
{
}

DeviceOutput::DeviceOutput(const char *name)
	: Device(name)
{
}

void DeviceInput::update_value(int new_value)
{
	if (value == new_value)
		return;

	this->value = new_value;
	LOG_INFO("Dev %s: value changed to %d", this->name, value);
}

int Device::jsonify(char *buffer, int buffer_len)
{
	int len = snprintf(buffer,
					   buffer_len,
					   R"({"name":"%s","value":%d})",
					   name,
					   value);
	if (len >= buffer_len) {
		LOG_ERROR("Jsonify: too long on '%s'", name);
		return 0;
	}
	return len;
}

int Device::get_value() const
{
	return value;
}
