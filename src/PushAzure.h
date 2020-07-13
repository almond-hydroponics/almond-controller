#pragma once

//Includes---------------------------------------------------------------------
#include <AzureIoTHub.h>
#include <AzureIoTProtocol_MQTT.h>
#include <iothubtransportmqtt.h>
#include <WiFiClientSecure.h>
#include "Device.h"
#include "TimerOverride.h"
#include "ApplicationConstants.h"

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "Wire.h"

#include <AzureIoTHub.h>
#include <AzureIoTProtocol_MQTT.h>
#include <iothubtransportmqtt.h>

#include "Globals.h"
#include <cstdio>


class PushAzure
{
public:
	PushAzure();

	/// @param values must be array of all fields in proper order. returns true on success (pushes every now and then, not every call)
	/// @param values_n - how many values to be pushed
	/// @param force_update - do we force update
	bool pushAzure(const int *values, int index_start, int values_n, bool force_update);
	static void sendCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void *userContextCallback);
	void sendMessage(IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle, char *buffer, bool temperatureAlert);
	IOTHUBMESSAGE_DISPOSITION_RESULT receiveMessageCallback(IOTHUB_MESSAGE_HANDLE message, void *userContextCallback);
	int deviceMethodCallback(const char *methodName, const unsigned char *payload, size_t size, unsigned char **response, size_t *response_size, void *userContextCallback);
	void twinCallback(DEVICE_TWIN_UPDATE_STATE updateState, const unsigned char *payLoad, size_t size, void *userContextCallback);

	void start();
	void stop();

	static const int push_interval_s = 15 * 60;
	static const int buffer_size = 4096;

protected:
	bool messagePending = false;
	bool messageSending = true;

	TimerOverride timerOverride;
	bool pushAzureRaw(const int *values, int index_start, int values_n, char *buffer);
};

//extern const PushAzure PUSH_AZURE;
