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

#undef read
#undef write

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

fileMgr::fileMgr()
{
	logger.StartLog("FILE IO");
}

fileMgr::~fileMgr()
{
	files.clear();
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
		logger.LogWarn("Could not load file ID " + ID + " SDL_Error: " + SDL_GetError());
		return false;
	}

	files.insert({fileID,std::move(newF)});
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
	if(result != 0)
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
	if(result != 0)
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

// Terminating precompiler directives  ////////////////////////////////////////
