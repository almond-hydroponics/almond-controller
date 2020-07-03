#pragma once

//Includes---------------------------------------------------------------------
#include <cstdarg>


//Types------------------------------------------------------------------------
void serial_setup(const char *name, int baudrate);

char *serial_receive(int *buffer_len);

int serial_receive_number(int min_value, int max_value);

void serial_print(const char *format, ...);

void serial_print_raw(const char *buffer, int len, bool line_change);
