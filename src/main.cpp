#define MAJOR_VER "00"
#define MINOR_VER "01"

//Includes---------------------------------------------------------------------
#include "AlmondPrecompiled.h"
#include <ESP8266WiFi.h>
#include "ApplicationConstants.h"
#include "ApplicationSetup.h"
#include "SetupWifi.h"
#include "Logger.h"
#include "SecureCredentials.h"
#include "Globals.h"
#include "../lib/sht-sensor-lib/sht21.h"


#ifndef STASSID
#define STASSID CONSTANTS.wlan.ssid
#define STAPSK  CONSTANTS.wlan.password
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

//Declarations-----------------------------------------------------------------
SetupWifi setupWifi(
	ssid,
	password,
	CA_CERT_PROG,
	CLIENT_CERT_PROG,
	CLIENT_KEY_PROG
);

SHT21 sht;

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
	ApplicationSetup::setup();
	sht.begin();
	setupWifi.setupWifi();
}

void loop()
{
	LOG.loop();
	setupWifi.loopWifi();
	// if wifi is not ready, don't do any other processing
	if (!setupWifi.isReadyForProcessing()) return;

	float humid = sht.readHumidity();
	float temp = sht.readTemperature();

	Serial.printf("Temp: %f, Humid: %f \n", temp, humid);
	delay(2000);
}
