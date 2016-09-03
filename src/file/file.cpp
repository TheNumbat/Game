
#include "file.h"
#include "..\log\log.h"
#include <SDL.h>
#include <dirent.h>

_file::_file() {
	sdl_file = NULL;
	good = false;
}

_file::~_file() {
	free();
}

bool _file::load(const std::string& path, filetype type, fileaccess access) {
	logSetContext("FILE");

	free();

	sdl_file = SDL_RWFromFile(path.c_str(),File::getAccessString(type,access).c_str());
	assert(sdl_file);
	if (!sdl_file) {
		logWarn("Failed to load file from path " +  path);
		return false;
	}

	good = true;
	return true;
}

bool _file::free() {
	if (good) {
		SDL_RWclose((SDL_RWops*)sdl_file);
		sdl_file = NULL;
		good = false;
		return true;
	}
	return false;
}

File::File() {

}

File::~File() {
	for (auto i : files)
		delete i.second;
	for (auto i : libs)
		SDL_UnloadObject(i.second);
}

bool File::init() {
	logSetContext("FILE");
	logInfo("File IO initialized.");
	return true;
}

bool File::kill() {
	logSetContext("FILE");
	logInfo("File IO destroyed.");
	return true;
}

bool File::loadFile(const std::string& fID, const std::string& path, filetype type, fileaccess access) {
	logSetContext("FILE");

	if (files.find(fID) != files.end()) {
		logWarn("File ID " + fID + " already in use!");
		return false;
	}

	_file* newF = new _file();
	if (!newF->load(path,type,access)) {
		logWarn("Could not load file ID " + fID + " Error: " + SDL_GetError());
		return false;
	}

	files.insert({fID, newF});
	logInfo("Loaded file ID " + fID + " from path " + path);
	return true;
}

bool File::freeFile(const std::string& fID) {
	logSetContext("FILE");

	auto fEntry = files.find(fID);
	if (fEntry == files.end()) {
		logWarn("Could not find to free file ID " + fID);
		return false;
	}

	delete fEntry->second;
	files.erase(fEntry);
	return true;
}

bool File::read(const std::string& fID, void* buf, u32 size, u32 numobjs) {
	logSetContext("FILE");

	auto fEntry = files.find(fID);
	if (fEntry == files.end()) {
		logWarn("Could not find file to read ID " + fID);
		return false;
	}

	s32 result = SDL_RWread((SDL_RWops*)fEntry->second->sdl_file,buf,size,numobjs);
	assert(result != 0);
	if (result == 0) {
		logWarn("Failed to read from file ID " + fID + " Error: " + SDL_GetError());
		return false;
	}

	return true;	
}

bool File::write(const std::string& fID, void* buf, u32 size, s32 numobjs) {
	logSetContext("FILE");

	auto fEntry = files.find(fID);
	if (fEntry == files.end()) {
		logWarn("Could not find file to write ID " + fID);
		return false;
	}

	s32 result = SDL_RWwrite((SDL_RWops*)fEntry->second->sdl_file,buf,size,numobjs);
	assert(result == numobjs);
	if (result < numobjs) {
		logWarn("Failed to write to file ID " + fID + " Error: " + SDL_GetError());
		return false;
	}

	return true;
}

u64 File::getOffset(const std::string& fID) {
	logSetContext("FILE");

	auto fEntry = files.find(fID);
	if (fEntry == files.end()) {
		logWarn("Could not find file to get offset ID " + fID);
		return false;
	}

	u64 result = SDL_RWtell((SDL_RWops*)fEntry->second->sdl_file);
	assert(result != -1);
	if (result == -1) {
		logWarn("Failed to get offset from file ID " + fID + " Error: " + SDL_GetError());
	}

	return result;
}

u64 File::setOffset(const std::string& fID, s64 offset, fileseek start) {
	logSetContext("FILE");

	auto fEntry = files.find(fID);
	if (fEntry == files.end()) {
		logWarn("Could not find file to set offset ID " + fID);
		return false;
	}

	s32 whence;
	if (start == file_start)
		whence = RW_SEEK_SET;
	else if (start == file_current)
		whence = RW_SEEK_CUR;
	else
		whence = RW_SEEK_END;

	u64 result = SDL_RWseek((SDL_RWops*)fEntry->second->sdl_file,offset,whence);
	assert(result != -1);
	if (result == -1) {
		logWarn("Failed to set offset from file ID " + fID + " Error: " + SDL_GetError());
	}
	return result;
}

bool File::loadLib(const std::string& lID, const std::string& path) {
	logSetContext("FILE");

	if (libs.find(lID) != libs.end()) {
		logWarn("library ID " + lID + " already in use!");
		return false;
	}

	void* lib = SDL_LoadObject(path.c_str());
	assert(lib);
	if (!lib) {
		logWarn("Could not load library from path " + path);
		return false;
	}

	libs.insert({lID, lib});
	logInfo("Loaded library ID " + lID + " from " + path);
	return true;
}

bool File::loadLibFolder(const std::string& path) {
	logSetContext("FILE");

	DIR *directory;
	dirent *entry;
	std::string dirPath = path;

	if (dirPath.back() != '/' && dirPath.back() != '\\') {
		dirPath.append("/");
	}

	directory = opendir(dirPath.c_str());
	assert(directory);
	if (!directory) {
		logWarn("Failed to open library directory at " + dirPath);
		return false;
	}

	logInfo("Loading library directory at " + dirPath);
	logEnterSec();

	while(entry = readdir(directory)) {
		std::string entryName = entry->d_name;

		if (entryName != ".." && entryName != ".") {
			std::string ext = entryName.substr(entryName.length() - 4, entryName.length());
			if (ext == ".dll")
				loadLib(entryName.substr(0, entryName.length() - 4), dirPath + entryName);
			else if (ext[0] != '.')
				loadLibFolder(dirPath + entryName + '/');
		}
	}

	logExitSec();
	return true;
}

bool File::freeLib(const std::string& lID) {
	logSetContext("FILE");

	auto entry = libs.find(lID);
	if (entry == libs.end()) {
		logWarn("Library ID " + lID + " not found!");
		return false;
	}

	SDL_UnloadObject(entry->second);
	libs.erase(entry);
	return true;
}

void* File::loadFunc(const std::string& lID, const std::string& func) {
	logSetContext("FILE");

	auto entry = libs.find(lID);
	if (entry == libs.end()) {
		logWarn("Library ID " + lID + " not found!");
		return NULL;
	}

	void* fPtr = SDL_LoadFunction(entry->second,func.c_str());
	assert(fPtr);
	if (!fPtr) {
		logWarn("Could not load function " + func + " from library " + lID);
		return NULL;
	}

	return fPtr;
}

std::vector<std::string> File::getNamesInFolder(const std::string& path) {
	logSetContext("FILE");

	DIR *directory;
	dirent *entry;
	std::string dirPath = path;
	std::vector<std::string> names;

	if (dirPath.back() != '/' && dirPath.back() != '\\') {
		dirPath.append("/");
	}

	directory = opendir(dirPath.c_str());
	assert(directory);
	if (!directory) {
		logWarn("Failed to open directory at " + dirPath);
		return names;
	}

	logInfo("Getting names from directory at " + dirPath);
	logEnterSec();

	while(entry = readdir(directory)) {
		std::string entryName = entry->d_name;

		if (entryName != ".." && entryName != ".")
			names.push_back(entryName.substr(entryName.find_last_of("/\\") + 1, entryName.find_last_of(".") - entryName.find_last_of("/\\") - 1));
	}

	logExitSec();
	return names;
}

// TODO: remove windows
#include <windows.h>
bool File::copyFile(const std::string& src, const std::string& dest) {
	logSetContext("FILE");

	s32 result = CopyFileA(src.c_str(), dest.c_str(), false);
	if (result == 0) {
		logWarn("Failed to copy file from " + src + " to " + dest + " Erorr: " + std::to_string(GetLastError()));
		return false;
	}
	return true;
}

std::string File::getAccessString(filetype type, fileaccess access) {
	std::string result;

	switch(access) {
		case file_read:
			result = "r";
			break;
		case file_empty_write:
			result = "w";
			break;
		case file_append:
			result = "a";
			break;
		case file_read_write:
			result = "r+";
			break;
		case file_empty_read_write:
			result = "w+";
			break;
		case file_read_append:
			result = "a+";
			break;
	}

	switch(type) {
		case file_text:
			result += "t";
			break;
		case file_binary:
			result += "b";
			break;
	}

	return result;
}