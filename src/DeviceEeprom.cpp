// Includes---------------------------------------------------------------------
#include <EEPROM.h>

#include "DeviceEeprom.h"
//#include "SerialReader.h"

bool needEraseEEPROM()
{
	char result = 'n';
	//	readFromSerial("Do you need re-input your credential information?(Auto
	//skip this after 5 seconds)[Y/n]", &result, 1, 5000);

	if (result == 'Y' || result == 'y')
	{
		clearParam();
		return true;
	}
	return false;
}

void clearParam()
{
	char data[EEPROM_SIZE];
	memset(data, '\0', EEPROM_SIZE);
	writeEEPROM(0, data, EEPROM_SIZE);
}

void writeEEPROM(int address, char* data, int size)
{
	EEPROM.begin(EEPROM_SIZE);

	// write the start maker
	EEPROM.write(address, EEPROM_START);
	address++;

	for (int i = 0; i < size; i++)
	{
		EEPROM.write(address, data[i]);
		address++;
	}

	EEPROM.write(address, EEPROM_END);
	EEPROM.commit();
	EEPROM.end();
}

int readEEPROM(int address, char* buff)
{
	EEPROM.begin(EEPROM_SIZE);
	int count = -1;
	char c = EEPROM.read(address);
	address++;

	if (c != EEPROM_START)
		return 0;

	while (c != EEPROM_END && count < EEPROM_SIZE)
	{
		c = (char)EEPROM.read(address);
		count++;
		address++;
		buff[count] = c;
	}
	EEPROM.end();

	return count;
}
