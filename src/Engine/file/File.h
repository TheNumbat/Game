#pragma once

#include "..\common.h"
#include <map>
#include <vector>

enum filetype {
	file_text,
	file_binary
};

enum fileaccess {
	file_read,
	file_empty_write,
	file_append,
	file_read_write,
	file_empty_read_write,
	file_read_append
};

enum fileseek {
	file_start,
	file_current,
	file_end
};

class _file {
	_file();
	~_file();

	bool load(const std::string& path, filetype type, fileaccess access);
	bool free();

	void* sdl_file;
	bool good;

	friend class File;
};

class File
{
public:
	ENGINE_API File();
	ENGINE_API ~File();

	ENGINE_API bool init();
	ENGINE_API bool kill();

	ENGINE_API bool loadFile(const std::string& fID, const std::string& path, filetype type, fileaccess access);
	ENGINE_API bool freeFile(const std::string& fID);

	ENGINE_API bool read(const std::string& fID, void* buf, u32 size, u32 numobjs = 1);
	ENGINE_API bool write(const std::string& fID, void* buf, u32 size, s32 numobjs = 1);

	ENGINE_API u64 getOffset(const std::string& fID);
	ENGINE_API u64 setOffset(const std::string& fID, s64 offset, fileseek start = file_start);

	ENGINE_API bool loadLib(const std::string& lID, const std::string& path);
	ENGINE_API bool loadLibFolder(const std::string& path);
	ENGINE_API bool freeLib(const std::string& lID);

	ENGINE_API void* loadFunc(const std::string& lID, const std::string& func);

	ENGINE_API std::vector<std::string> getNamesInFolder(const std::string& path);
	ENGINE_API bool copyFile(const std::string& src, const std::string& dest);

	static std::string getAccessString(filetype type, fileaccess access);

private:
	std::map<std::string, _file*> files;
	std::map<std::string, void*> libs;
};

