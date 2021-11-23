//Includes---------------------------------------------------------------------
#include "AlmondConfiguration.h"

const int16_t I2C_MASTER = 0X42;
const int16_t I2C_SLAVE = 0x08;

AlmondConfiguration ALMOND_CONFIGURATION;


//Implementation---------------------------------------------------------------
void AlmondConfiguration::setup()
{
	AlmondConfiguration::setup_i2c();
	AlmondConfiguration::setup_fs();
	AlmondConfiguration::setup_led_outputs();
}

unsigned int AlmondConfiguration::get_free_heap()
{
	return ESP.getFreeHeap();
}

void AlmondConfiguration::setup_i2c()
{
	Wire.begin(PIN_SDA, PIN_SCL);
}

void AlmondConfiguration::setup_fs()
{
	bool mounted = SPIFFS.begin();
	if (mounted) {
		LOG_INFO("FS ok.");
	}
	else {
		LOG_FATAL("FS mount failed!");
	}
}

void AlmondConfiguration::setup_led_outputs()
{
	LOG.setup_led(WIFI_LED);
	LOG.setup_led(PIN_LED);
	LOG.setup_led(PIN_PUMP);
	LOG_INFO("Output LEDs ok.");
}
