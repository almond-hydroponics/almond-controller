// Includes---------------------------------------------------------------------
#include "DevicePinInput.h"

// Implementation---------------------------------------------------------------
DevicePinInput::DevicePinInput(const char* name, uint8_t pin,
                               uint8_t filter_len, bool invert)
		: DeviceInput(name), filter_sum(0), pin(pin), filter_len(filter_len),
			invert(invert)
{
	this->value = -1;
}

void DevicePinInput::setup() { pinMode(this->pin, INPUT_PULLUP); }

void DevicePinInput::loop()
{
	if (!timer.check(poll_interval))
		return;

	timer.reset();

	int val = digitalRead(this->pin);
	val = !invert == (val != 0);

	this->filter_sum += (val * 2 - 1);

	if (this->filter_sum > filter_len / 2)
	{
		this->update_value(1);

		if (this->filter_sum > filter_len)
		{
			this->filter_sum = filter_len;
		}
	}
	else if (this->filter_sum < -(filter_len / 2))
	{
		this->update_value(0);

		if (this->filter_sum < -filter_len)
		{
			this->filter_sum = -filter_len;
		}
	}
	else
	{
		this->update_value(-1);
	}
}
