// Program Information ////////////////////////////////////////////////////////

/**
	@file fileMgr.h

	@brief Implementation file for file manager class.

	Event manager stores and translates events.

	@author Max Slater

	@version 1.00 (28 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "fileMgr.h"
#include <SDL.h>
#include <dirent.h>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

fileMgr::fileMgr()
{
	logger.StartLog("FILE IO");
	logger.LogInfo("File IO initialized");	
}

fileMgr::~fileMgr()
{
	files.clear();
	for(auto& entry : libraries)
	{
		SDL_UnloadObject(entry.second);
	}
	logger.LogInfo("File IO deinitialized");
}

bool fileMgr::loadFile(const std::string& path, int8 type, const std::string& access, const std::string& ID)
{
	std::string fileID;

	if(!ID.size())
	{
		fileID = path.substr(path.find_last_of("/\\") + 1, path.find_last_of(".") - path.find_last_of("/\\") - 1);
	}
	else
	{
		fileID = ID;
	}

	if(files.find(ID) != files.end())
	{
		logger.LogWarn("File ID " + ID + " already in use!");
		return false;
	}

	std::unique_ptr<file> newF = std::make_unique<file>();
	if(!newF->load(path,type,access))
	{
		logger.LogWarn("Could not load file ID " + fileID + " SDL_Error: " + SDL_GetError());
		return false;
	}

	files.insert({fileID,std::move(newF)});
	logger.LogInfo("Loaded file ID " + fileID + " from path " + path);
	return true;
}

bool fileMgr::freeFile(const std::string& ID)
{
	auto fEntry = files.find(ID);
	if(fEntry == files.end())
	{
		logger.LogWarn("Could not find file ID " + ID);
		return false;
	}

	files.erase(fEntry);
	return true;
}

bool fileMgr::read(const std::string& ID, void* buffer, uint32 size, uint32 numobjs)
{
	auto fEntry = files.find(ID);
	if(fEntry == files.end())
	{
		logger.LogWarn("Could not find file ID " + ID);
		return false;
	}

	int result = SDL_RWread((SDL_RWops*)fEntry->second->sdl_file,buffer,size,numobjs);
	if(result == 0)
	{
		logger.LogWarn("Failed to read from file ID " + ID + " SDL_Error: " + SDL_GetError());
		return false;
	}

	return true;	
}

bool fileMgr::write(const std::string& ID, void* buffer, uint32 size, uint32 numobjs)
{
	auto fEntry = files.find(ID);
	if(fEntry == files.end())
	{
		logger.LogWarn("Could not find file ID " + ID);
		return false;
	}

	int result = SDL_RWwrite((SDL_RWops*)fEntry->second->sdl_file,buffer,size,numobjs);
	if(result < numobjs)
	{
		logger.LogWarn("Failed to write to file ID " + ID + " SDL_Error: " + SDL_GetError());
		return false;
	}

	return true;
}

uint64 fileMgr::getOffset(const std::string& ID)
{
	auto fEntry = files.find(ID);
	if(fEntry == files.end())
	{
		logger.LogWarn("Could not find file ID " + ID);
		return false;
	}

	uint64 result = SDL_RWtell((SDL_RWops*)fEntry->second->sdl_file);
	if(result == -1)
	{
		logger.LogWarn("Failed to get offset from file ID " + ID + " SDL_Error: " + SDL_GetError());
	}
	return result;
}

uint64 fileMgr::setOffset(const std::string& ID, uint64 offset, fileseek start)
{
	auto fEntry = files.find(ID);
	if(fEntry == files.end())
	{
		logger.LogWarn("Could not find file ID " + ID);
		return false;
	}

	int32 whence;
	if(start == file_start)
	{
		whence = RW_SEEK_SET;
	}
	else if(start == file_current)
	{
		whence = RW_SEEK_CUR;
	}
	else
	{
		whence = RW_SEEK_END;
	}

	uint64 result = SDL_RWseek((SDL_RWops*)fEntry->second->sdl_file,offset,whence);
	if(result == -1)
	{
		logger.LogWarn("Failed to set offset from file ID " + ID + " SDL_Error: " + SDL_GetError());
	}
	return result;
}

bool fileMgr::loadLibrary(const std::string& path, const std::string& ID)
{
	if(libraries.find(ID) != libraries.end())
	{
		logger.LogWarn("library ID " + ID + " already in use!");
		return false;
	}

	std::string libID;
	if(!ID.size())
	{
		libID = path.substr(path.find_last_of("/\\") + 1, path.find_last_of(".") - path.find_last_of("/\\") - 1);
	}
	else
	{
		libID = ID;
	}

	void* lib = SDL_LoadObject(path.c_str());
	if(!lib)
	{
		logger.LogWarn("Could not load library from path " + path);
		return false;
	}

	libraries.insert({libID,lib});
	logger.LogInfo("Loaded library ID " + libID + " from " + path);
	return true;
}

bool fileMgr::freeLibrary(const std::string& ID)
{
	auto entry = libraries.find(ID);
	if(entry == libraries.end())
	{
		logger.LogWarn("Library ID " + ID + " not found!");
		return false;
	}

	SDL_UnloadObject(entry->second);
	libraries.erase(entry);
	return true;
}

void* fileMgr::loadFunction(const std::string& libID, const std::string& funcName)
{
	auto entry = libraries.find(libID);
	if(entry == libraries.end())
	{
		logger.LogWarn("Library ID " + libID + " not found!");
		return NULL;
	}

	void* func = SDL_LoadFunction(entry->second,funcName.c_str());
	if(!func)
	{
		logger.LogWarn("Could not load function " + funcName + " from library " + libID);
		return NULL;
	}

	return func;
}

bool fileMgr::loadLibraryRec(const std::string& path)
{
	DIR *directory;
	struct dirent *entry;
	std::string dirPath = path;

	if(dirPath.back() != '/' && dirPath.back() != '\\')
	{
		dirPath.append("/");
	}

	// Open directory
	directory = opendir(dirPath.c_str());
	if(!directory)
	{
		logger.LogWarn("Failed to open library directory at " + dirPath);
		return false;
	}

	logger.LogInfo("Loading library directory at " + dirPath);
	logger.EnterSec();

	// Read directory
	while(entry = readdir(directory))
	{
		std::string entryName = entry->d_name;

		if(entryName != ".." && entryName != ".")
		{
			// Load texture or load another directory
			/// @todo make this better, it just tests for a .extension
			if(entryName[entryName.size() - 4] == '.')
			{
				loadLibrary(dirPath + entryName);
			}
			else
			{
				loadLibraryRec(dirPath + entryName + '/');
			}
		}
	}

	logger.ExitSec();

	// Success
	return true;
}

std::vector<std::string> fileMgr::getNames(const std::string& path)
{
	DIR *directory;
	struct dirent *entry;
	std::string dirPath = path;
	std::vector<std::string> names;

	if(dirPath.back() != '/' && dirPath.back() != '\\')
	{
		dirPath.append("/");
	}

	// Open directory
	directory = opendir(dirPath.c_str());
	if(!directory)
	{
		logger.LogWarn("Failed to open directory at " + dirPath);
		return names;
	}

	logger.LogInfo("Getting names from directory at " + dirPath);
	logger.EnterSec();

	// Read directory
	while(entry = readdir(directory))
	{
		std::string entryName = entry->d_name;

		if(entryName != ".." && entryName != ".")
		{
			names.push_back(entryName.substr(entryName.find_last_of("/\\") + 1, entryName.find_last_of(".") - entryName.find_last_of("/\\") - 1));
		}
	}

	logger.ExitSec();

	// Success
	return names;
}

// Terminating precompiler directives  ////////////////////////////////////////
