#ifndef SUPA_NTP_CIENT_H
#define SUPA_NTP_CIENT_H


void ntp_client_loop();
void ntp_client_setup();

/// @returns current time since epoc in seconds if time has been aquired, otherwise will return 
/// relative time from the boot
unsigned long ntp_client_gettime();

/// @returns true if the time has been synced from server. false means that this is local time.
bool ntp_client_valid();

#endif
