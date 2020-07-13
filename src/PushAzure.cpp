//Includes---------------------------------------------------------------------
#include "PushAzure.h"


const char *onSuccess = "\"Successfully invoke device method\"";
const char *notFound = "\"No method found\"";

//Implementation---------------------------------------------------------------
PushAzure::PushAzure()
{
	this->timerOverride.reset();
}

void blinkLED()
{
	digitalWrite(PIN_LED, HIGH);
	delay(500);
	digitalWrite(PIN_LED, LOW);
}

void PushAzure::start()
{
	LOG_INFO("Start sending temperature and humidity data.");
	this->messageSending = true;
}

void PushAzure::stop()
{
	LOG_INFO("Stop sending temperature and humidity data.");
	this->messageSending = false;
}

void PushAzure::sendCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void *userContextCallback)
{
	if (IOTHUB_CLIENT_CONFIRMATION_OK == result) {
		LOG_INFO("Message sent to Azure IoT Hub.");
		blinkLED();
	} else {
		LOG_ERROR("Failed to send message to Azure IoT Hub.");
	}
//	this->messagePending = false;
}

void PushAzure::sendMessage(IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle, char *buffer, bool temperatureAlert)
{
	IOTHUB_MESSAGE_HANDLE messageHandle = IoTHubMessage_CreateFromByteArray((const unsigned char *)buffer, strlen(buffer));
	if (messageHandle == nullptr) {
		LOG_INFO("Unable to create a new IoTHubMessage.");
	}
	else {
		MAP_HANDLE properties = IoTHubMessage_Properties(messageHandle);
		Map_Add(properties, "temperatureAlert", temperatureAlert ? "true" : "false");
		LOG_INFO("Sending message: %s.\r\n", buffer);

		if (IoTHubClient_LL_SendEventAsync(iotHubClientHandle, messageHandle, sendCallback, nullptr) != IOTHUB_CLIENT_OK)
		{
			this->messagePending = false;
			LOG_INFO("Failed to hand over the message to IoTHubClient.");
		}
		else
		{
			this->messagePending = true;
			LOG_INFO("IoTHubClient accepted the message for delivery.");
		}
		IoTHubMessage_Destroy(messageHandle);
	}
}

IOTHUBMESSAGE_DISPOSITION_RESULT PushAzure::receiveMessageCallback(IOTHUB_MESSAGE_HANDLE message, void *userContextCallback)
{
	int* counter = (int*)userContextCallback;
	IOTHUBMESSAGE_DISPOSITION_RESULT result;
	const unsigned char *buffer;
	size_t size;
	if (IoTHubMessage_GetByteArray(message, &buffer, &size) != IOTHUB_MESSAGE_OK) {
		LOG_ERROR("Unable to get IoTHubMessage_GetByteArray.");
		result = IOTHUBMESSAGE_REJECTED;
	} else {
		/*buffer is not zero terminated*/
		char *temp = (char *)malloc(size + 1);

		if (temp == nullptr) {
			return IOTHUBMESSAGE_ABANDONED;
		}

		strncpy(temp, (const char *)buffer, size);
		temp[size] = '\0';
		LOG_INFO("Receive C2D message: %s.\r\n", temp);
		free(temp);
		blinkLED();
	}
	(*counter)++;
	return IOTHUBMESSAGE_ACCEPTED;
}

int PushAzure::deviceMethodCallback(
	const char *methodName,
	const unsigned char *payload,
	size_t size,
	unsigned char **response,
	size_t *response_size,
	void *userContextCallback
)
{
	LOG_INFO("Try to invoke method %s.\r\n", methodName);
	(void)userContextCallback;
	const char *responseMessage = onSuccess;
	int result = 200;

	if (strcmp(methodName, "start") == 0)
	{
		PushAzure::start();
	}
	else if (strcmp(methodName, "stop") == 0)
	{
		PushAzure::stop();
	}
	else
	{
		LOG_ERROR("No method %s found.\r\n", methodName);
		responseMessage = notFound;
		result = 404;
	}

	*response_size = strlen(responseMessage);
	*response = (unsigned char *)malloc(*response_size);
	strncpy((char *)(*response), responseMessage, *response_size);

	return result;
}

void PushAzure::twinCallback(
	DEVICE_TWIN_UPDATE_STATE updateState,
	const unsigned char *payLoad,
	size_t size,
	void *userContextCallback
)
{
	(void)userContextCallback;
	char *temp = (char *)malloc(size + 1);
	for (int i = 0; i < size; i++) {
		temp[i] = (char)(payLoad[i]);
	}
	temp[size] = '\0';
//	parseTwinMessage(temp);
	free(temp);
}
