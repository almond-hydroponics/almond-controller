#pragma once


//Includes---------------------------------------------------------------------
#include <PubSubClient.h>
#include "SetupWifi.h"
//#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
//#include <ESP8266HTTPUpdateServer.h>

//#define WebServer ESP8266WebServer
//#define ESPmDNS ESP8266mDNS
//#define ESPHTTPUpdateServer ESP8266HTTPUpdateServer

#define MAX_TOPIC_SUBSCRIPTION_LIST_SIZE 10
#define MAX_DELAYED_EXECUTION_LIST_SIZE 10
#define CONNECTION_RETRY_DELAY 10 * 1000

void onConnectionEstablished(); // MUST be implemented in your sketch. Called once everything is connected (Wifi, mqtt).

typedef std::function<void()> ConnectionEstablishedCallback;
typedef std::function<void(const String &message)> MessageReceivedCallback;
typedef std::function<void(const String &topicStr, const String &message)>
	MessageReceivedCallbackWithTopic;
typedef std::function<void()> DelayedExecutionCallback;

//Types------------------------------------------------------------------------
class MqttServer
{
public:
	// MQTT without MQTT authentication
	MqttServer(
		const char *mqttServerIp,
		const char *mqttClientName,
		short mqttServerPort,
		const WiFiClient& wifiClient);

	// MQTT with MQTT authentication
	MqttServer(
		const char *mqttServerIp,
		const char *mqttUsername,
		const char *mqttPassword,
		const char *mqttClientName,
		short mqttServerPort,
		const WiFiClient& wifiClient);

	virtual ~MqttServer() = default;

	// Optional functionality
//	void enableDebuggingMessages(const bool enabled = true); // Allow to display useful debugging messages. Can be set to false to disable them during program execution
//	void enableHTTPWebUpdater(const char *username, const char *password, const char *address = "/"); // Activate the web updater, must be set before the first loop() call.
//	void enableHTTPWebUpdater(const char *address = "/mqtt"); // Will set user and password equal to mqttUsername and mqttPassword
	void enableMQTTPersistence(); // Tell the broker to establish a persistent connection. Disabled by default. Must be called before the first loop() execution
	void enableLastWillMessage(const char *topic, const char *message, const bool retain = false); // Must be set before the first loop() call.

	// Main loop, to call at each sketch loop()
	void loop();

	// MQTT related
	bool publish(const String &topic, const String &payload, bool retain = false);
	bool subscribe(const String &topic, MessageReceivedCallback messageReceivedCallback);
	bool subscribe(const String &topic, MessageReceivedCallbackWithTopic messageReceivedCallback);
	bool unsubscribe(const String &topic);   //Unsubscribes from the topic, if it exists, and removes it from the CallbackList.

	// Other
	void executeDelayed(unsigned long delay, DelayedExecutionCallback callback);

	inline bool isConnected();
//  inline bool isWifiConnected() const { return mWifiConnected; }; // Return true if wifi is connected
	inline bool isMqttConnected() const { return mqttConnected; }; // Return true if mqtt is connected
	inline bool getConnectionEstablishedCount() const { return connectionEstablishedCount; }; // Return the number of time onConnectionEstablished has been called since the beginning.
	inline void setOnConnectionEstablishedCallback(ConnectionEstablishedCallback callback)
	{
		connectionEstablishedCallback = callback;
	}; // Default to onConnectionEstablished, you might want to override this for special cases like two MQTT connections in the same sketch

private:
	// MQTT related
	unsigned long lastMqttConnectionMillis;
	const char *mqttServerIp;
	const char *mqttUsername;
	const char *mqttPassword;
	const char *mqttClientName;
	int mqttServerPort;
	char *mqttLastWillTopic;
	char *mqttLastWillMessage;
	bool mqttConnected;
	bool mqttCleanSession;
	bool mqttLastWillRetain;

	PubSubClient mqttClient;

	struct TopicSubscriptionRecord
	{
		String topic;
		MessageReceivedCallback callback;
		MessageReceivedCallbackWithTopic callbackWithTopic;
	};
	TopicSubscriptionRecord topicSubscriptionList[MAX_TOPIC_SUBSCRIPTION_LIST_SIZE];
	byte topicSubscriptionListSize;

	// HTTP update server related
//	char *updateServerAddress;
//	char *updateServerUsername{};
//	char *updateServerPassword{};
//	WebServer *httpServer;
//	ESPHTTPUpdateServer *httpUpdater;

	// Delayed execution related
	struct DelayedExecutionRecord
	{
		unsigned long targetMillis;
		DelayedExecutionCallback callback;
	};
	DelayedExecutionRecord delayedExecutionList[MAX_DELAYED_EXECUTION_LIST_SIZE];
	byte delayedExecutionListSize;

	// General behaviour related
	ConnectionEstablishedCallback connectionEstablishedCallback;
	bool showLegacyConstructorWarning;
	unsigned int connectionEstablishedCount; // Incremented before each connectionEstablishedCallback call

	void connectToMqttBroker();
	static bool mqttTopicMatch(const String &topic1, const String &topic2);
	void mqttMessageReceivedCallback(char *topic, byte *payload, unsigned int length);
};
