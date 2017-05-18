
/**
This is automatically generate mock file (from tests/fakes/FS.h). If you wish to edit this, move this to mocks_man directory.
*/
#ifndef _AUTOMOCK_FS_H
#define _AUTOMOCK_FS_H


#include "fff.h"
#include "FS.h"

using namespace fs;

size_t File__write(  File*, const uint8_t*, size_t );
DECLARE_FAKE_VALUE_FUNC( size_t, File__write,  File*, const uint8_t*, size_t );
int File__available(  File* );
DECLARE_FAKE_VALUE_FUNC( int, File__available,  File* );
int File__peek(  File* );
DECLARE_FAKE_VALUE_FUNC( int, File__peek,  File* );
void File__flush(  File* );
DECLARE_FAKE_VOID_FUNC( File__flush,  File* );
size_t File__read(  File*, uint8_t*, size_t );
DECLARE_FAKE_VALUE_FUNC( size_t, File__read,  File*, uint8_t*, size_t );
bool File__seek(  File*, uint32_t, SeekMode );
DECLARE_FAKE_VALUE_FUNC( bool, File__seek,  File*, uint32_t, SeekMode );
size_t File__position( const File* );
DECLARE_FAKE_VALUE_FUNC( size_t, File__position, const File* );
size_t File__size( const File* );
DECLARE_FAKE_VALUE_FUNC( size_t, File__size, const File* );
void File__close(  File* );
DECLARE_FAKE_VOID_FUNC( File__close,  File* );
const char* File__name( const File* );
DECLARE_FAKE_VALUE_FUNC( const char*, File__name, const File* );
File Dir__openFile(  Dir*, const char* );
DECLARE_FAKE_VALUE_FUNC( File, Dir__openFile,  Dir*, const char* );
String Dir__fileName(  Dir* );
DECLARE_FAKE_VALUE_FUNC( String, Dir__fileName,  Dir* );
size_t Dir__fileSize(  Dir* );
DECLARE_FAKE_VALUE_FUNC( size_t, Dir__fileSize,  Dir* );
bool Dir__next(  Dir* );
DECLARE_FAKE_VALUE_FUNC( bool, Dir__next,  Dir* );
bool FS__begin(  FS* );
DECLARE_FAKE_VALUE_FUNC( bool, FS__begin,  FS* );
void FS__end(  FS* );
DECLARE_FAKE_VOID_FUNC( FS__end,  FS* );
bool FS__format(  FS* );
DECLARE_FAKE_VALUE_FUNC( bool, FS__format,  FS* );
bool FS__info(  FS*, FSInfo );
DECLARE_FAKE_VALUE_FUNC( bool, FS__info,  FS*, FSInfo );
File FS__open(  FS*, const char*, const char* );
DECLARE_FAKE_VALUE_FUNC( File, FS__open,  FS*, const char*, const char* );
bool FS__exists(  FS*, const char* );
DECLARE_FAKE_VALUE_FUNC( bool, FS__exists,  FS*, const char* );
Dir FS__openDir(  FS*, const char* );
DECLARE_FAKE_VALUE_FUNC( Dir, FS__openDir,  FS*, const char* );
bool FS__remove(  FS*, const char* );
DECLARE_FAKE_VALUE_FUNC( bool, FS__remove,  FS*, const char* );
bool FS__rename(  FS*, const char*, const char* );
DECLARE_FAKE_VALUE_FUNC( bool, FS__rename,  FS*, const char*, const char* );


#endif // _AUTOMOCK_FS_H
