#define MAJOR_VER "00"
#define MINOR_VER "01"

//Includes---------------------------------------------------------------------
#include "AlmondPrecompiled.h"
#include "ApplicationConstants.h"
#include "ApplicationSetup.h"
#include "Logger.h"


//Implementation---------------------------------------------------------------
static void logger_fatal_hook(const char *log_line)
{
	// if we are not connected, we are not storing the messages for now.
	if (!WIFI_SETUP.connected())
		return;

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
	LOG.setup_serial(CONSTANTS.hostname, CONSTANTS.baudrate);
	LOG.setup_fatal_hook(logger_fatal_hook);
	ApplicationSetup::setup();
}

void loop()
{
	LOG.loop();
}
