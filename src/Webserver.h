#pragma once

// Includes---------------------------------------------------------------------
#include <ESP8266WebServer.h>

// Types------------------------------------------------------------------------
void webserver_loop();
void webserver_setup();
char* webserver_get_buffer();

constexpr const int WEBSERVER_MAX_RESPONSE_SIZE = (4 * 1024);

extern ESP8266WebServer server;
