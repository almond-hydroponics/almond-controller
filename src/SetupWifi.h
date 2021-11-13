#pragma once


//Includes---------------------------------------------------------------------
#include <utility>

#include "AlmondPrecompiled.h"
#include "ApplicationConstants.h"
#include "AsyncWait.h"
#include "DeviceRtc.h"
#include "Logger.h"
#include "Globals.h"


//Types------------------------------------------------------------------------
class SetupWifi
{
public:
	SetupWifi(
		const char *ssid_,
		const char *password_,
		const char *ca_cert_PROG_,
		const char *client_cert_PROG_,
		const char *client_key_PROG_
		)
		: ssid(ssid_)
		, password(password_)
		, ca_cert_PROG(ca_cert_PROG_)
		, client_cert_PROG(client_cert_PROG_)
		, client_key_PROG(client_key_PROG_)

		, x509CaCert(ca_cert_PROG)
		, x509ClientCert(client_cert_PROG)
		, PrivateClientKey(client_key_PROG)
	{
		wifiClient.setTrustAnchors(&x509CaCert);
		wifiClient.setClientRSACert(&x509ClientCert, &PrivateClientKey);
	}

	virtual ~SetupWifi() = default;

	WiFiClient &getWiFiClient()
	{
		return wifiClient;
	}

	bool isReadyForProcessing()
	{
		return (setClock_status > FINISHED);
	}

	static String getMacAddress();
	void setupWifi();
	void loopWifi();

private:
	const char *ssid;
	const char *password;
	const char *ca_cert_PROG;
	const char *client_cert_PROG;
	const char *client_key_PROG;    // KEEP THIS VALUE PRIVATE AND SECURE!!!

	BearSSL::WiFiClientSecure wifiClient;
	BearSSL::X509List x509CaCert;
	BearSSL::X509List x509ClientCert;
	BearSSL::PrivateKey PrivateClientKey;

	// Note: variables of type 'status' are NEVER assigned a value of 'FINISHED'.
	// 'FINISHED' is only used if comparisons (e.g. if(x_status>FINISHED)... ),
	// rather than always testing for SUCCESS || FAILED.
	enum status
	{
		NOT_STARTED, STARTED, FINISHED, SUCCESS, FAILED
	};
	status setClock_status = NOT_STARTED;
	AsyncWait setClock_AsyncWait;

	void setClock();
	void checkClockStatus();
};
