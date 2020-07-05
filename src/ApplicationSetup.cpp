//Includes---------------------------------------------------------------------
#include "AlmondPrecompiled.h"
#include "ApplicationSetup.h"
#include "ApplicationConstants.h"
#include "Logger.h"

const int16_t I2C_MASTER = 0X42;
const int16_t I2C_SLAVE = 0x08;

ApplicationSetup APPLICATION_SETUP;

void ApplicationSetup::setup()
{
	ApplicationSetup::setup_i2c();
}

unsigned int ApplicationSetup::get_free_heap()
{
	return ESP.getFreeHeap();
}

void ApplicationSetup::setup_i2c()
{
	Wire.begin(PIN_SDA, PIN_SCL, I2C_MASTER);
}
