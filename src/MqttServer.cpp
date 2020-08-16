//Includes---------------------------------------------------------------------
#include "MqttServer.h"

//#include <utility>
#include "Logger.h"


//Implementation---------------------------------------------------------------
MqttServer::MqttServer(
	const char *ssid,
	const char *password,
	const char *caCertProg,
	const char *clientCertProg,
	const char *clientKeyProg,
	const char *mqttServerIp,
	const char *mqttUsername,
	const char *mqttPassword,
	const char *mqttClientName,
	const short mqttServerPort)
	:
	SetupWifi(
		ssid,
		password,
		caCertProg,
		clientCertProg,
		clientKeyProg),
		mqttServerIp(mqttServerIp),
		mqttUsername(mqttUsername),
		mqttPassword(mqttPassword),
		mqttClientName(mqttClientName),
		mqttServerPort(mqttServerPort),
		mqttClient(mqttServerIp, mqttServerPort,SetupWifi::getWiFiClient())
{
	// MQTT client
	topicSubscriptionListSize = 0;
	mqttConnected = false;
	lastMqttConnectionMillis = 0;
	mqttLastWillTopic = nullptr;
	mqttLastWillMessage = nullptr;
	mqttLastWillRetain = false;
	mqttCleanSession = true;
	mqttClient.setCallback([this](char *topic, byte *payload, unsigned int length) {
		this->mqttMessageReceivedCallback(topic, payload, length);
	});

	// Web updater
	updateServerAddress = nullptr;
	httpServer = nullptr;
	httpUpdater = nullptr;

	// other
	enableSerialLogs = false;
	connectionEstablishedCallback = onConnectionEstablished;
	showLegacyConstructorWarning = false;
	delayedExecutionListSize = 0;
	connectionEstablishedCount = 0;
}

MqttServer::~MqttServer()
{
	if (httpServer != nullptr)
		delete httpServer;
	if (httpUpdater != nullptr)
		delete httpUpdater;
}

// Return true if everything is connected
bool MqttServer::isConnected()
{
	return SetupWifi::isReadyForProcessing() & isMqttConnected();
}

// Configuration functions, most of them must be called before the first loop() call
void MqttServer::enableDebuggingMessages(const bool enabled)
{
	enableSerialLogs = enabled;
}

void MqttServer::enableHTTPWebUpdater(
	const char *username,
	const char *password,
	const char *address)
{
	if (httpServer == nullptr) {
		httpServer = new WebServer(80);
		httpUpdater = new ESPHTTPUpdateServer(enableSerialLogs);
		updateServerUsername = (char *)username;
		updateServerPassword = (char *)password;
		updateServerAddress = (char *)address;
	}
	else if (enableSerialLogs)
		LOG_WARN("SYS! You can't call enableHTTPWebUpdater() more than once !\n");
}

void MqttServer::enableHTTPWebUpdater(const char *address)
{
	LOG_INFO("Setup MQTT from HTTP Web");
	if (mqttUsername == nullptr || mqttPassword == nullptr)
		enableHTTPWebUpdater("", "", address);
	else
		LOG_INFO("Updating MQTT from HTTP Web");
		enableHTTPWebUpdater(mqttUsername, mqttPassword, address);
}

void MqttServer::enableMQTTPersistence()
{
	mqttCleanSession = false;
}

void MqttServer::enableLastWillMessage(
	const char *topic,
	const char *message,
	const bool retain)
{
	LOG_INFO("Enabling last will message");
	mqttLastWillTopic = (char *)topic;
	mqttLastWillMessage = (char *)message;
	mqttLastWillRetain = retain;
}

void MqttServer::loop()
{
	LOG_INFO("Starting mqtt server...");
	unsigned long currentMillis = millis();
	Serial.println(isReadyForProcessing());
	Serial.println(SetupWifi::isReadyForProcessing());

	if (isReadyForProcessing()) {
		// Config of web updater
		if (httpServer != nullptr) {
			MDNS.begin(mqttClientName);
			httpUpdater->setup(
				httpServer,
			updateServerAddress,
			updateServerUsername,
			updateServerPassword);
			httpServer->begin();
			MDNS.addService("http", "tcp", 80);

			if (enableSerialLogs)
				LOG_INFO(
					"WEB: Updater ready, open http://%s.local in your browser and login with username '%s' and password '%s'.\n",
					mqttClientName,
					updateServerUsername,
					updateServerPassword);
		}

		// MQTT handling
		if (mqttClient.connected()) {
			LOG_INFO("MQTT connected");
			mqttClient.loop();
		}
		else {
			if (mqttConnected) {
				if (enableSerialLogs)
					LOG_ERROR("MQTT! Lost connection.");

				topicSubscriptionListSize = 0;
				mqttConnected = false;
			}

			if (currentMillis - lastMqttConnectionMillis
				> CONNECTION_RETRY_DELAY || lastMqttConnectionMillis == 0)
				connectToMqttBroker();
		}

		// Web updater handling
		if (httpServer != nullptr) {
			httpServer->handleClient();
#ifdef ESP8266
			MDNS.update(); // We need to do this only for ESP8266
#endif
		}
	}

	// Delayed execution handling
	if (delayedExecutionListSize > 0) {
		unsigned long currentMillis = millis();

		for (byte i = 0; i < delayedExecutionListSize; i++) {
			if (delayedExecutionList[i].targetMillis <= currentMillis) {
				delayedExecutionList[i].callback();
				for (byte j = i; j < delayedExecutionListSize - 1; j++)
					delayedExecutionList[j] = delayedExecutionList[j + 1];
				delayedExecutionListSize--;
				i--;
			}
		}
	}

	// Old constructor support warning
	if (enableSerialLogs && showLegacyConstructorWarning) {
		showLegacyConstructorWarning = false;
		LOG_ERROR(
			"SYS! You are using a constructor that will be deleted soon, please update your code with the new construction format.\n");
	}
}

bool MqttServer::publish(const String &topic, const String &payload, bool retain)
{
	bool success = mqttClient.publish(topic.c_str(), payload.c_str(), retain);

	if (enableSerialLogs) {
		if (success)
			LOG_INFO("MQTT << [%s] %s\n", topic.c_str(), payload.c_str());
		else
			// This can occurs if the message is too long according to the maximum defined in PubsubClient.h
			LOG_ERROR("MQTT! publish failed, is the message too long ?");
	}
	return success;
}

bool MqttServer::subscribe(
	const String &topic,
	MessageReceivedCallback messageReceivedCallback)
{
	// Check the possibility to add a new topic
	if (topicSubscriptionListSize >= MAX_TOPIC_SUBSCRIPTION_LIST_SIZE) {
		if (enableSerialLogs)
			LOG_INFO("MQTT! Subscription list is full, ignored.");
		return false;
	}

	// Check the duplicate of the subscription to the topic
	bool found = false;
	for (byte i = 0; i < topicSubscriptionListSize && !found; i++)
		found = topicSubscriptionList[i].topic.equals(topic);

	if (found) {
		if (enableSerialLogs)
			LOG_INFO("MQTT! Subscribed to [%s] already, ignored.\n",
					 topic.c_str());
		return false;
	}

	// All checks are passed - do the job
	bool success = mqttClient.subscribe(topic.c_str());

	if (success)
		topicSubscriptionList[topicSubscriptionListSize++] =
			{topic, std::move(messageReceivedCallback), nullptr};

	if (enableSerialLogs) {
		if (success)
			LOG_INFO("MQTT: Subscribed to [%s]\n", topic.c_str());
		else
			LOG_ERROR("MQTT! subscribe failed");
	}

	return success;
}

bool MqttServer::subscribe(
	const String &topic,
	MessageReceivedCallbackWithTopic messageReceivedCallback)
{
	if (subscribe(topic, (MessageReceivedCallback)nullptr)) {
		topicSubscriptionList[topicSubscriptionListSize - 1]
			.callbackWithTopic = messageReceivedCallback;
		return true;
	}
	return false;
}

bool MqttServer::unsubscribe(const String &topic)
{
	bool found = false;
	bool success = false;

	for (byte i = 0; i < topicSubscriptionListSize; i++) {
		if (!found) {
			if (topicSubscriptionList[i].topic.equals(topic)) {
				found = true;
				success = mqttClient.unsubscribe(topic.c_str());

				if (enableSerialLogs) {
					if (success)
						LOG_INFO("MQTT: Unsubscribed from %s\n",
								 topic.c_str());
					else
						LOG_ERROR("MQTT! unsubscribe failed");
				}
			}
		}

		if (found) {
			if ((i + 1) < MAX_TOPIC_SUBSCRIPTION_LIST_SIZE)
				topicSubscriptionList[i] = topicSubscriptionList[i + 1];
		}
	}

	if (found)
		topicSubscriptionListSize--;
	else if (enableSerialLogs)
		LOG_ERROR("MQTT! Topic cannot be found to unsubscribe, ignored.");

	return success;
}

void MqttServer::executeDelayed(const unsigned long delay,
								DelayedExecutionCallback callback)
{
	if (delayedExecutionListSize < MAX_DELAYED_EXECUTION_LIST_SIZE) {
		DelayedExecutionRecord delayedExecutionRecord;
		delayedExecutionRecord.targetMillis = millis() + delay;
		delayedExecutionRecord.callback = callback;

		delayedExecutionList[delayedExecutionListSize] =
			delayedExecutionRecord;
		delayedExecutionListSize++;
	}
	else if (enableSerialLogs)
		LOG_INFO("SYS! The list of delayed functions is full.\n");
}


// ================== Private functions ====================-

void MqttServer::connectToMqttBroker()
{
	if (enableSerialLogs)
		LOG_INFO("MQTT: Connecting to broker @%s ... ", mqttServerIp);

	if (mqttClient.connect(
		mqttClientName,
		mqttUsername,
		mqttPassword,
		mqttLastWillTopic,
		0,
		mqttLastWillRetain,
		mqttLastWillMessage,
		mqttCleanSession)) {
		mqttConnected = true;

		if (enableSerialLogs)
			LOG_INFO("Ok.");

		connectionEstablishedCount++;
		connectionEstablishedCallback();
	}
	else if (enableSerialLogs) {
		LOG_ERROR("Unable to connect, ");

		switch (mqttClient.state()) {
			case -4:LOG_INFO("MQTT_CONNECTION_TIMEOUT");
				break;
			case -3:LOG_INFO("MQTT_CONNECTION_LOST");
				break;
			case -2:LOG_INFO("MQTT_CONNECT_FAILED");
				break;
			case -1:LOG_INFO("MQTT_DISCONNECTED");
				break;
			case 1:LOG_INFO("MQTT_CONNECT_BAD_PROTOCOL");
				break;
			case 2:LOG_INFO("MQTT_CONNECT_BAD_CLIENT_ID");
				break;
			case 3:LOG_INFO("MQTT_CONNECT_UNAVAILABLE");
				break;
			case 4:LOG_INFO("MQTT_CONNECT_BAD_CREDENTIALS");
				break;
			case 5:LOG_INFO("MQTT_CONNECT_UNAUTHORIZED");
				break;
		}
	}

	lastMqttConnectionMillis = millis();
}

/**
 * Matching MQTT topics, handling the eventual presence of a single wildcard character
 *
 * @param topic1 is the topic may contain a wildcard
 * @param topic2 must not contain wildcards
 * @return true on MQTT topic match, false otherwise
 */
bool MqttServer::mqttTopicMatch(const String &topic1, const String &topic2)
{
	//Serial.println(String("Comparing: ") + topic1 + " and " + topic2);
	int i = 0;
	if ((i = topic1.indexOf('#')) >= 0) {
		//Serial.print("# detected at position "); Serial.println(i);
		String t1a = topic1.substring(0, i);
		String t1b = topic1.substring(i + 1);
		//Serial.println(String("t1a: ") + t1a);
		//Serial.println(String("t1b: ") + t1b);
		if ((t1a.length() == 0 || topic2.startsWith(t1a)) &&
			(t1b.length() == 0 || topic2.endsWith(t1b)))
			return true;
	}
	else if ((i = topic1.indexOf('+')) >= 0) {
		//Serial.print("+ detected at position "); Serial.println(i);
		String t1a = topic1.substring(0, i);
		String t1b = topic1.substring(i + 1);
		//Serial.println(String("t1a: ") + t1a);
		//Serial.println(String("t1b: ") + t1b);
		if ((t1a.length() == 0 || topic2.startsWith(t1a)) &&
			(t1b.length() == 0 || topic2.endsWith(t1b))) {
			if (topic2.substring(t1a.length(), topic2.length() - t1b.length())
				.indexOf('/') == -1)
				return true;
		}
	}
	else {
		return topic1.equals(topic2);
	}
	return false;
}

void MqttServer::mqttMessageReceivedCallback(
	char *topic,
	byte *payload,
	unsigned int length)
{
	// Convert the payload into a String
	// First, We ensure that we dont bypass the maximum size of the PubSubClient library buffer that originated the payload
	// This buffer has a maximum length of MQTT_MAX_PACKET_SIZE and the payload begin at "headerSize + topicLength + 1"
	unsigned int strTerminationPos;
	if (strlen(topic) + length + 9 >= MQTT_MAX_PACKET_SIZE) {
		strTerminationPos = length - 1;

		if (enableSerialLogs)
			LOG_INFO(
				"MQTT! Your message may be truncated, please change MQTT_MAX_PACKET_SIZE of PubSubClient.h to a higher value.\n");
	}
	else
		strTerminationPos = length;

	// Second, we add the string termination code at the end of the payload and we convert it to a String object
	payload[strTerminationPos] = '\0';
	String payloadStr((char *)payload);
	String topicStr(topic);

	// Logging
	if (enableSerialLogs)
		LOG_INFO("MQTT >> [%s] %s\n", topic, payloadStr.c_str());

	// Send the message to subscribers
	for (byte i = 0; i < topicSubscriptionListSize; i++) {
		if (mqttTopicMatch(topicSubscriptionList[i].topic, String(topic))) {
			if (topicSubscriptionList[i].callback != nullptr)
				topicSubscriptionList[i].callback(payloadStr); // Call the callback
			if (topicSubscriptionList[i].callbackWithTopic != nullptr)
				topicSubscriptionList[i].callbackWithTopic(topicStr, payloadStr); // Call the callback
		}
	}
}
