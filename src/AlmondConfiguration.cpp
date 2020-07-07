//Includes---------------------------------------------------------------------
#include "AlmondConfiguration.h"

const int16_t I2C_MASTER = 0X42;
const int16_t I2C_SLAVE = 0x08;

AlmondConfiguration ALMOND_CONFIGURATION;


//Implementation---------------------------------------------------------------
void AlmondConfiguration::setup()
{
	this->setup_i2c();
	this->setup_wifi();
}

void AlmondConfiguration::loop()
{
	this->loop_wifi();
}

unsigned int AlmondConfiguration::get_free_heap()
{
	return ESP.getFreeHeap();
}

void AlmondConfiguration::setup_i2c()
{
	Wire.begin(PIN_SDA, PIN_SCL);
}
