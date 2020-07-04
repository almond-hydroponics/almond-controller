#pragma once

//Includes---------------------------------------------------------------------

//Types------------------------------------------------------------------------
class ApplicationSetup
{
public:
	static void setup();
	static void loop();

	static unsigned int get_free_heap();

protected:
	static void setup_i2c();
	static void setup_ota();
	static void setup_fs();

	static void loop_ota();
};

extern ApplicationSetup APPLICATION_SETUP;
