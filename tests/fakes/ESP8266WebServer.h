#ifndef FAKE_ESP8266WEBSERVER_H
#define FAKE_ESP8266WEBSERVER_H

#include <functional>
#include <vector>

#include <ESP8266WiFi.h>

#include "FS.h"

// Following is COPY PASTE from the real header

enum HTTPMethod { HTTP_ANY, HTTP_GET, HTTP_POST, HTTP_PUT, HTTP_PATCH, HTTP_DELETE, HTTP_OPTIONS };
enum HTTPUploadStatus { UPLOAD_FILE_START, UPLOAD_FILE_WRITE, UPLOAD_FILE_END,
                        UPLOAD_FILE_ABORTED };
enum HTTPClientStatus { HC_NONE, HC_WAIT_READ, HC_WAIT_CLOSE };

#define HTTP_DOWNLOAD_UNIT_SIZE 1460
#define HTTP_UPLOAD_BUFLEN 2048
#define HTTP_MAX_DATA_WAIT 1000 //ms to wait for the client to send the request
#define HTTP_MAX_POST_WAIT 1000 //ms to wait for POST data to arrive
#define HTTP_MAX_CLOSE_WAIT 2000 //ms to wait for the client to close the connection

#define CONTENT_LENGTH_UNKNOWN ((size_t) -1)
#define CONTENT_LENGTH_NOT_SET ((size_t) -2)

class ESP8266WebServer;

typedef struct {
  HTTPUploadStatus status;
  String  filename;
  String  name;
  String  type;
  size_t  totalSize;    // file size
  size_t  currentSize;  // size of data currently in buf
  uint8_t buf[HTTP_UPLOAD_BUFLEN];
} HTTPUpload;




class ESP8266WebServer
{
public:
  ESP8266WebServer(int port = 80);
  ~ESP8266WebServer();

  void begin();
  void handleClient();

  typedef std::function<void(void)> THandlerFunction;
  typedef std::pair< std::string, THandlerFunction> THandlerPair;
  void on(const char* uri, THandlerFunction handler);
  void serveStatic(const char* uri, fs::FS& fs, const char* path, const char* cache_header = NULL );
  void onNotFound(THandlerFunction fn);  //called when handler is not assigned

  void send(int code, const char* content_type = NULL, const String& content = String(""));
  
  bool _test_started;
  int         _test_sent_code;
  std::string _test_sent_content;
  std::string _test_sent_header;

  bool _test_serve( const char* url );
  void _test_clear();
private:
   std::vector< THandlerPair > handlers;   
};

#endif