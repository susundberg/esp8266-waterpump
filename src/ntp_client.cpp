/*
 From : https://github.com/esp8266/Arduino - with modifications
 
 Udp NTP Client

 created 4 Sep 2010
 by Michael Margolis
 modified 9 Apr 2012
 by Tom Igoe

 This code is in the public domain.

 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "ntp_client.h"
#include "stimer.h"

static const char* NTP_TIMESERVER  = "time.nist.gov"; // time.nist.gov NTP server
static const unsigned int NTP_LOCAL_PORT = 18888;       // local port to listen for UDP packets
static const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message


struct NTP_Runtime
{
   public:
     unsigned long time();
     void loop();
     void setup();
     
   private:
     bool valid;
     bool wait_for_package;
     void send_request();
     
     STimer timer_send;
     unsigned long time_ntp_s;
     unsigned long time_relative_s;
     
     byte buffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
     WiFiUDP udp;
} ;


static NTP_Runtime RUNTIME;

void ntp_client_setup()
{
   memset( &RUNTIME, 0x00, sizeof(RUNTIME));
   RUNTIME.setup();
}

void ntp_client_loop()
{
  RUNTIME.loop();
}

void NTP_Runtime::setup()
{
   this->udp.begin( NTP_LOCAL_PORT );
}

const unsigned long NTP_QUERY_UPDATE_INTERVAL_S = (60*60);
const unsigned long NTP_QUERY_RETRY_INTERVAL_S  = 15;

void NTP_Runtime::loop()
{
  bool send_request = false;
  
  if ( this->valid == false )
  {  // we have not received valid time since start, lets be more aggressive here
     
     if ( this->timer_send.check( NTP_QUERY_RETRY_INTERVAL_S*1000L ))
        send_request = true;
  }
  else
  {
     if ( this->timer_send.check( NTP_QUERY_UPDATE_INTERVAL_S*1000L ) )
     {
        send_request = true;
     }
  }

  if ( send_request == true )
  {
     this->send_request( );
     this->timer_send.reset();
     this->wait_for_package = true;
  }
  
  if ( this->wait_for_package == false )
     return;
  
  // Do we have package ready  
  if ( this->udp.parsePacket() == false )
     return;
  
  this->wait_for_package = false;
  
  // We've received a packet, read the data from it
  this->udp.read( this->buffer, NTP_PACKET_SIZE); // read the packet into the buffer

  //the timestamp starts at byte 40 of the received packet and is four bytes,
  // or two words, long. First, esxtract the two words:
  unsigned long highWord = word(this->buffer[40], this->buffer[41]);
  unsigned long lowWord = word(this->buffer[42], this->buffer[43]);
  // combine the four bytes (two words) into a long integer
  // this is NTP time (seconds since Jan 1 1900):
  unsigned long secsSince1900 = highWord << 16 | lowWord;
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  const unsigned long seventyYears = 2208988800UL;
  // subtract seventy years:
  unsigned long epoch = secsSince1900 - seventyYears;
  this->time_ntp_s = epoch;
  this->time_relative_s = (millis() + 500L) / 1000L;
  this->valid = true;
}

// send an NTP request to the time server at the given address
void NTP_Runtime::send_request()
{
  // set all bytes in the buffer to 0
  memset(this->buffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  this->buffer[0] = 0b11100011;   // LI, Version, Mode
  this->buffer[1] = 0;     // Stratum, or type of clock
  this->buffer[2] = 6;     // Polling Interval
  this->buffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  this->buffer[12]  = 49;
  this->buffer[13]  = 0x4E;
  this->buffer[14]  = 49;
  this->buffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  this->udp.beginPacket(NTP_TIMESERVER, 123); //NTP requests are to port 123
  this->udp.write(this->buffer, NTP_PACKET_SIZE);
  this->udp.endPacket();
}


