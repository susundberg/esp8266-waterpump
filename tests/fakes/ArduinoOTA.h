#ifndef SUPA_ARDUINOOTA_H
#define SUPA_ARDUINOOTA_H


#include <functional>

typedef enum {
  OTA_IDLE,
  OTA_WAITAUTH,
  OTA_RUNUPDATE
} ota_state_t;

typedef enum {
  OTA_AUTH_ERROR,
  OTA_BEGIN_ERROR,
  OTA_CONNECT_ERROR,
  OTA_RECEIVE_ERROR,
  OTA_END_ERROR
} ota_error_t;

class ArduinoOTAClass
{
public:
   
    typedef std::function<void(void)> THandlerFunction;
    typedef std::function<void(ota_error_t)> THandlerFunction_Error;
    typedef std::function<void(unsigned int, unsigned int)> THandlerFunction_Progress;

    ArduinoOTAClass();
    ~ArduinoOTAClass();
    void setPort(uint16_t port);
    void setHostname(const char *hostname);
    String getHostname();
    void setPassword(const char *password);
    void onStart(THandlerFunction fn);
    void onEnd(THandlerFunction fn);
    void onError(THandlerFunction_Error fn);
    void onProgress(THandlerFunction_Progress fn);
    void begin();
    void handle();

};

extern ArduinoOTAClass ArduinoOTA;

#endif