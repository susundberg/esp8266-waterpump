#include "config.h"

Config CONFIG = {
   .hostname = "waterpump",
   .wlan = {
     .sid  = "WLAN_SSID";
     .pass = "WLAN_PASSWD";
   },   
   .email = { 
      .server_host = "smtp.gmail.com"
      .server_port = 465, 
      .login = "gmail_login@gmail.com"
      .password = "email_password"
   },
};
