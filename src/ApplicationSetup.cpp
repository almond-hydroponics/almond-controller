//Includes---------------------------------------------------------------------
#include "AlmondPrecompiled.h"
#include "ApplicationSetup.h"
#include "ApplicationConstants.h"
#include "Logger.h"

ApplicationSetup APPLICATION_SETUP;

void ApplicationSetup::setup()
{
	ApplicationSetup::setup_i2c();
	ApplicationSetup::setup_fs();
	ApplicationSetup::setup_ota();
}

void ApplicationSetup::loop()
{
	ApplicationSetup::loop_ota();
}

unsigned int ApplicationSetup::get_free_heap()
{
	return ESP.getFreeHeap();
}

void ApplicationSetup::setup_ota()
{
	ArduinoOTA.setHostname(CONSTANTS.hostname);

	// No authentication by default
	ArduinoOTA.setPassword(CONSTANTS.password);

	ArduinoOTA.onStart([]()
	{
	   LOG_WARN("Update start!");
	   SPIFFS.end();
	});

	ArduinoOTA.onEnd([]()
	{
	 LOG_WARN("Update done!");
	});

	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
	{
		static unsigned int last = 0xFF;
		unsigned int prog = (progress / (total / 100));

		if (prog == last) return;

		last = prog;
		LOG_INFO("Progress: %u%%", prog);
	});

	ArduinoOTA.onError([](ota_error_t error)
	{
		LOG_FATAL("Error %u during update: ", error);
	});
	ArduinoOTA.begin();
}

void ApplicationSetup::loop_ota()
{
	ArduinoOTA.handle();
}

void ApplicationSetup::setup_fs()
{
	bool mounted = SPIFFS.begin();
	if (mounted) {
		LOG_INFO("FS mounted.");
	}
	else {
		LOG_FATAL("FS mounting failed!");
	}
}

void ApplicationSetup::setup_i2c()
{
	Wire.begin(PIN_SDA, PIN_SCL);
}
