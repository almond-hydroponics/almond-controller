//Includes---------------------------------------------------------------------
#include "EnvironmentSht.h"


//Implementation---------------------------------------------------------------
EnvironmentSht::EnvironmentSht(const char *name)
	: DeviceInput(name)
{
}

void EnvironmentSht::setup()
{
	sht.begin();
}

void EnvironmentSht::loop()
{
	if (!timer.check(measure_interval)) return;
	timer.reset();

	if (this->name[0] == 't') {
		value = this->sht.readTemperature();
	} else {
		value = this->sht.readHumidity();
	}
}
