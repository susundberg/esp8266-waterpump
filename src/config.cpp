#include "config.h"


// Note: G++ seems to require that these are in the proper order (same as declared).
const Config CONFIG = {
   .email = { 
      .server_host = "smtp.gmail.com",
      .server_port = 465, 
      .login = "gmail_login@gmail.com",
      .password = "email_password",
      .receiver = "any_email@any.com",
   },
   .wlan = {
     .sid  = "WLAN_SSID",
     .pass = "WLAN_PASSWD",
   },   
   .hostname = "waterpump",
   .password = "waterize"
};
