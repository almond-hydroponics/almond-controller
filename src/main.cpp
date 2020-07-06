#define MAJOR_VER "00"
#define MINOR_VER "01"

//Includes---------------------------------------------------------------------
#include "AlmondPrecompiled.h"
#include <ESP8266WiFi.h>
#include "ApplicationConstants.h"
#include "ApplicationSetup.h"
#include "ApplicationLogic.h"
#include "DeviceRtc.h"
#include "WaterLevel.h"
#include "EnvironmentSht.h"
#include "SetupWifi.h"
#include "Webserver.h"
#include "Logger.h"
#include "SecureCredentials.h"
#include "Globals.h"


#ifndef STASSID
#define STASSID CONSTANTS.wlan.ssid
#define STAPSK  CONSTANTS.wlan.password
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

//Declarations-----------------------------------------------------------------
DeviceRtc			DEV_RTC("rtc");
WaterLevel			DEV_WLEVEL("water_level", PIN_TRIGGER, PIN_ECHO);
EnvironmentSht		DEV_TEMP("temp");
EnvironmentSht		DEV_HUMID("humid");
ApplicationLogic	APPLICATION_LOGIC;

SetupWifi setupWifi(
	ssid,
	password,
	CA_CERT_PROG,
	CLIENT_CERT_PROG,
	CLIENT_KEY_PROG
);

Device *const DEVICES[] = { &DEV_TEMP, &DEV_HUMID, &DEV_WLEVEL };

//Implementation---------------------------------------------------------------
static void logger_fatal_hook(const char *log_line)
{
	// if we are not connected, we are not storing the messages for now.
	if (!setupWifi.isReadyForProcessing()) return;

	int buffer_len = Logger::max_line_len + 128;
	int subject_len = 256;
	char *buffer = (char *)malloc(buffer_len);
	char *subject = (char *)malloc(subject_len);

	memset(buffer, 0x00, buffer_len);
	memset(subject, 0x00, subject_len);

	// out of memory, lets skip the whole thing.
	if (buffer == nullptr || subject == nullptr)
		return;

	snprintf(
		buffer,
		buffer_len - 1,
		"Error on %s: %s.\n",
		CONSTANTS.hostname,
		log_line
	);
	snprintf(
		subject,
		subject_len - 1,
		"[ESP] %s : error detected",
		CONSTANTS.hostname
	);

//	email_send(&CONSTANTS.email, CONSTANTS.email.receiver, subject, buffer);

	free(subject);
	free(buffer);
}

void setup()
{
	// SETUP ESP8266 DEVICE
#ifdef DEBUG
	LOG.setup_serial(CONSTANTS.hostname, CONSTANTS.baudrate);
#endif

	LOG.setup_led(PIN_LED);
	LOG.setup_fatal_hook(logger_fatal_hook);
	APPLICATION_SETUP.setup();
	setupWifi.setupWifi();

	for (auto loop : DEVICES) loop->setup();

	Logger::set_status(Logger::Status::RUNNING);

	WEBSERVER.on("/get/dev",)
}

void loop()
{
	LOG.loop();
	static TimerOverride update_timer;
	static bool update_when_elapsed = false;
	static unsigned long avail_memory_last = 0xFFFF;
	unsigned long avail_memory_now = APPLICATION_SETUP.get_free_heap();

	if (avail_memory_now < avail_memory_last) {
		LOG_INFO("Memory: %u", avail_memory_now);
		avail_memory_last = avail_memory_now;
	}

	setupWifi.loopWifi();
	// if wifi is not ready, don't do any other processing
	if (!setupWifi.isReadyForProcessing()) return;

	delay(10);

	for (auto loop : DEVICES) loop->loop();

	Config_run_table_time time_now{};
	DEV_RTC.time_of_day(&time_now);

	Serial.printf("Temp: %d, Humid: %d \n", DEV_TEMP.get_value(), DEV_HUMID.get_value());
	Serial.printf("Distance: %d \n", DEV_WLEVEL.get_value());
	Serial.printf("Time: %wint_t \n", &time_now);
//	Serial.printf("Temp: %d \n", DEV_TEMP.get_value());
	delay(2000);
}
