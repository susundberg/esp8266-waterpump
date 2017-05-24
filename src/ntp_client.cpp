/*
 From: https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/NTPClient/NTPClient.ino
 
 Udp NTP Client
 Get the time from a Network Time Protocol (NTP) time server
 Demonstrates use of UDP sendPacket and ReceivePacket
 For more on NTP time servers and the messages needed to communicate with them,
 see http://en.wikipedia.org/wiki/Network_Time_Protocol
 created 4 Sep 2010
 by Michael Margolis
 modified 9 Apr 2012
 by Tom Igoe
 updated for the ESP8266 12 Apr 2015 
 by Ivan Grokhotkov
 This code is in the public domain.
 
 
 Modified by SUPA
 */

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


#include "logger.h"


#define NTP_LOCAL_PORT 239      // local port to listen for UDP packets
#define NTP_SERVER_HOST "time.nist.gov"
#define NTP_PACKET_SIZE 48 // NTP time stamp is in the first 48 bytes of the message



// send an NTP request to the time server at the given address
void sendNTPpacket(WiFiUDP& udp, IPAddress& address, char* buffer )
{
  // set all bytes in the buffer to 0
  memset(buffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  buffer[0] = 0b11100011;   // LI, Version, Mode
  buffer[1] = 0;     // Stratum, or type of clock
  buffer[2] = 6;     // Polling Interval
  buffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  buffer[12]  = 49;
  buffer[13]  = 0x4E;
  buffer[14]  = 49;
  buffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(buffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

uint32_t ntp_update()
{
  LOG_INFO("NTP time update requested");
   
   // A UDP instance to let us send and receive packets over UDP
  WiFiUDP udp;
  IPAddress server_ip;
  char buffer[ NTP_PACKET_SIZE ];
  
  udp.begin( NTP_LOCAL_PORT );
  
  //get a random server from the pool
  WiFi.hostByName(NTP_SERVER_HOST, server_ip); 

  
  sendNTPpacket(udp, server_ip, buffer); // send an NTP packet to a time server
  
  LOG_INFO("Package sent, wait 1s");
  
  int receive_size = 0;
  // wait to see if a reply is available
  for ( int delay_loop = 0; delay_loop < 100; delay_loop ++ )
  {
     delay(100);
     receive_size = udp.parsePacket();
     
     if ( receive_size < NTP_PACKET_SIZE ) 
     {
        continue;
     }
     
     break;
  }
  
  if ( receive_size < NTP_PACKET_SIZE )
  {
     LOG_ERROR("NTP update failed, %d bytes received", receive_size );
     return 0;
  }
  
  LOG_INFO("Package received, len=%d", receive_size );

  // We've received a packet, read the data from it
  udp.read(buffer, NTP_PACKET_SIZE); // read the packet into the buffer

  //the timestamp starts at byte 40 of the received packet and is four bytes,
  // or two words, long. First, esxtract the two words:

  unsigned long highWord = word(buffer[40], buffer[41]);
  unsigned long lowWord = word(buffer[42], buffer[43]);
  // combine the four bytes (two words) into a long integer
  // this is NTP time (seconds since Jan 1 1900):
  unsigned long secsSince1900 = highWord << 16 | lowWord;
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  const unsigned long seventyYears = 2208988800UL;
  // subtract seventy years:
  unsigned long epoch = secsSince1900 - seventyYears;
  return epoch;
}
