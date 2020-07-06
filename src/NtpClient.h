#pragma once

//Includes---------------------------------------------------------------------
#include <cstdint>


//Types------------------------------------------------------------------------
/// @returns current time since epoch in seconds, on error returns 0
uint32_t ntp_update();
