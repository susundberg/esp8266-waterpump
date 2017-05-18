#ifndef FAKE_ESP8266FS_H
#define FAKE_ESP8266FS_H

#include "Arduino.h"

namespace fs {
   
enum SeekMode {
    SeekSet = 0,
    SeekCur = 1,
    SeekEnd = 2
};

struct FSInfo {
    size_t totalBytes;
    size_t usedBytes;
    size_t blockSize;
    size_t pageSize;
    size_t maxOpenFiles;
    size_t maxPathLength;
};

class File
{
   public:
    // Print methods:
    size_t write(const uint8_t *buf, size_t size) ;
    // Stream methods:
    int available() ;
    int peek() ;
    void flush() ;
    size_t read(uint8_t* buf, size_t size);
    bool seek(uint32_t pos, SeekMode mode);
    size_t position() const;
    size_t size() const;
    void close();
    operator bool() const;
    const char* name() const;
};

class Dir
{
public:
    File openFile(const char* mode);
    String fileName();
    size_t fileSize();
    bool next();
};


class FS
{
  public:

    bool begin();
    void end();
    
    bool format();
    bool info(FSInfo& info);

    File open(const char* path, const char* mode);
    bool exists(const char* path);
    Dir openDir(const char* path);
    bool remove(const char* path);

    bool rename(const char* pathFrom, const char* pathTo);

};

   
}; // namespace

extern fs::FS SPIFFS;

#endif