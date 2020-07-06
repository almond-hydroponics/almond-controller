//Includes---------------------------------------------------------------------
#include "WaterLevel.h"
#include "Logger.h"
#include "ApplicationConstants.h"
#include "Globals.h"


//Implementation---------------------------------------------------------------
WaterLevel::WaterLevel(
	const char *name,
	uint8_t pin_trigger,
	uint8_t pin_echo)
	:
	DeviceInput(name)
{
	this->pin_echo = pin_echo;
	this->pin_trigger = pin_trigger;
	this->value = 0;
	this->nfails = 0;
}

void WaterLevel::setup()
{
	pinMode(PIN_TRIGGER, OUTPUT);
	pinMode(PIN_ECHO, INPUT);
	digitalWrite(this->pin_trigger, LOW);
}

void WaterLevel::measure_pulse_in()
{
	constexpr const float speed_of_sound_scalar = 0.17175f;
	const unsigned long max_timeout =
		CONSTANTS.pump.low_level_height_mm / speed_of_sound_scalar;

	float duration = pulseIn(this->pin_echo, HIGH, max_timeout * 2);

	//Calculate the distance (in mm) based on the speed of sound.
	// c = 343.5 * 1000 / 1000000 = 0.3435 mm/ss
	// and we go there and back -> *.5

	float distance = (duration * speed_of_sound_scalar);

	if (distance == 0) {
		LOG_WARN("Distance measurement failed (%d row)!", nfails);
		nfails += 1;
		return;
	}

	// if the low height is 400mm and distance is 100mm our level is 300mm.
	distance = CONSTANTS.pump.low_level_height_mm - distance;
	nfails = 0;
	float ma_old = this->value;
	const float alpha = 0.5;

	if (this->value == 0) {
		// in the beginning, boost directly to proper level
		ma_old = distance;
	}

	float ma_new = alpha * distance + (1 - alpha) * ma_old;

	int new_value = (int)ma_new;

	this->value = new_value;
}

void WaterLevel::loop()
{
	if (nfails >= nfail_limit) {
		if (nfails == nfail_limit) {
			// log the error only once (to avoid spamming..)
			LOG_FATAL("Distance sensor is failing.");
			nfails = nfail_limit + 1;
		}
		this->value = 0;
		return;
	}

	if (!this->timer.check(measure_interval)) return;

	this->timer.reset();

	digitalWrite(this->pin_trigger, LOW);
	delayMicroseconds(2);
	digitalWrite(this->pin_trigger, HIGH);
	delayMicroseconds(10);
	digitalWrite(this->pin_trigger, LOW);
	measure_pulse_in();

//	const unsigned long duration = pulseIn(PIN_ECHO, HIGH);
//	int distance = duration/29/2;
//
//	if (duration == 0) {
//		DEBUG_LOGLN("Warning: No pulse from the sensor");
//		this->value = 0;
//	} else {
//		this->value = (int)distance;
//	}
}
