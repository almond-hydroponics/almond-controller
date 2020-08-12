#pragma once

//Includes---------------------------------------------------------------------
#include <AzureIoTHub.h>
#include <AzureIoTProtocol_MQTT.h>
#include <iothubtransportmqtt.h>
#include <WiFiClientSecure.h>
#include "Device.h"
#include "TimerOverride.h"
#include "ApplicationConstants.h"


//Types------------------------------------------------------------------------
class IotHubAzure
{
public:
	IotHubAzure();
	static void sendCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void *userContextCallback);

protected:
	bool messagePending = false;
	bool messageSending = true;

	TimerOverride timerOverride;
};
