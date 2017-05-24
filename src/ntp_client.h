#ifndef SUPA_NTP_CIENT_H
#define SUPA_NTP_CIENT_H


#include <stdint.h>

/// @returns current time since epoc in seconds, on error returns 0
uint32_t ntp_update();


#endif
