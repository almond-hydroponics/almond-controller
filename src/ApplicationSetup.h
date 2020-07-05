#pragma once

//Includes---------------------------------------------------------------------

//Types------------------------------------------------------------------------
class ApplicationSetup
{
public:
	static void setup();

	static unsigned int get_free_heap();

protected:
	static void setup_i2c();
};

extern ApplicationSetup APPLICATION_SETUP;
