
/**
This is automatically generated mock file, see header file for more details.
*/

#include "FS_fake.h"

FS SPIFFS;


DEFINE_FAKE_VALUE_FUNC( size_t, File__write,  File*, const uint8_t*, size_t );
size_t File::write(const uint8_t* buf, size_t size)  { return File__write(  this, buf, size ); }
DEFINE_FAKE_VALUE_FUNC( int, File__available,  File* );
int File::available()  { return File__available(  this ); }
DEFINE_FAKE_VALUE_FUNC( int, File__peek,  File* );
int File::peek()  { return File__peek(  this ); }
DEFINE_FAKE_VOID_FUNC( File__flush,  File* );
void File::flush()  { File__flush(  this ); }
DEFINE_FAKE_VALUE_FUNC( size_t, File__read,  File*, uint8_t*, size_t );
size_t File::read(uint8_t* buf, size_t size)  { return File__read(  this, buf, size ); }
DEFINE_FAKE_VALUE_FUNC( bool, File__seek,  File*, uint32_t, SeekMode );
bool File::seek(uint32_t pos, SeekMode mode)  { return File__seek(  this, pos, mode ); }
DEFINE_FAKE_VALUE_FUNC( size_t, File__position, const File* );
size_t File::position() const { return File__position(  this ); }
DEFINE_FAKE_VALUE_FUNC( size_t, File__size, const File* );
size_t File::size() const { return File__size(  this ); }
DEFINE_FAKE_VOID_FUNC( File__close,  File* );
void File::close()  { File__close(  this ); }
DEFINE_FAKE_VALUE_FUNC( const char*, File__name, const File* );
const char* File::name() const { return File__name(  this ); }
DEFINE_FAKE_VALUE_FUNC( File, Dir__openFile,  Dir*, const char* );
File Dir::openFile(const char* mode)  { return Dir__openFile(  this, mode ); }
DEFINE_FAKE_VALUE_FUNC( String, Dir__fileName,  Dir* );
String Dir::fileName()  { return Dir__fileName(  this ); }
DEFINE_FAKE_VALUE_FUNC( size_t, Dir__fileSize,  Dir* );
size_t Dir::fileSize()  { return Dir__fileSize(  this ); }
DEFINE_FAKE_VALUE_FUNC( bool, Dir__next,  Dir* );
bool Dir::next()  { return Dir__next(  this ); }
DEFINE_FAKE_VALUE_FUNC( bool, FS__begin,  FS* );
bool FS::begin()  { return FS__begin(  this ); }
DEFINE_FAKE_VOID_FUNC( FS__end,  FS* );
void FS::end()  { FS__end(  this ); }
DEFINE_FAKE_VALUE_FUNC( bool, FS__format,  FS* );
bool FS::format()  { return FS__format(  this ); }
DEFINE_FAKE_VALUE_FUNC( bool, FS__info,  FS*, FSInfo );
bool FS::info(FSInfo& info)  { return FS__info(  this, info ); }
DEFINE_FAKE_VALUE_FUNC( File, FS__open,  FS*, const char*, const char* );
File FS::open(const char* path, const char* mode)  { return FS__open(  this, path, mode ); }
DEFINE_FAKE_VALUE_FUNC( bool, FS__exists,  FS*, const char* );
bool FS::exists(const char* path)  { return FS__exists(  this, path ); }
DEFINE_FAKE_VALUE_FUNC( Dir, FS__openDir,  FS*, const char* );
Dir FS::openDir(const char* path)  { return FS__openDir(  this, path ); }
DEFINE_FAKE_VALUE_FUNC( bool, FS__remove,  FS*, const char* );
bool FS::remove(const char* path)  { return FS__remove(  this, path ); }
DEFINE_FAKE_VALUE_FUNC( bool, FS__rename,  FS*, const char*, const char* );
bool FS::rename(const char* pathFrom, const char* pathTo)  { return FS__rename(  this, pathFrom, pathTo ); }
