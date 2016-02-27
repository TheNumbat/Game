// Program Information ////////////////////////////////////////////////////////

/**
	@file soundMgr.cpp

	@brief Implementaiton file for soundMgr class

	Manges the outout of sound, simalarly to how graphicMgr manages the output
	of textures.

	@author Max Slater

	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "soundMgr.h"

#include <SDL_mixer.h>
#include <dirent.h>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

soundMgr::soundMgr() 
{
	good = false;
	logger.StartLog("SOUND");
}

soundMgr::~soundMgr() 
{
	sounds.clear();
	kill();
}

bool soundMgr::init() 
{
	if(good) 
	{
		logger.LogWarn("Cannot reinitialize sound!");
		return false;
	}

	// Open audio
	int result = Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
	assert(result == 0);
	if(result != 0) 
	{
		logger.LogFatal((std::string)"Failed to initalize sound SDL_Mixer Error: " + Mix_GetError());
		return false;
	}
	
	// Success
	good = true;
	logger.LogInfo("Sound initialized");
	return true;
}

bool soundMgr::kill() 
{
	if(!good)
	{
		logger.LogWarn("Cannot re-destroy sound!");
		return false;
	}

	stopAll();

	// Success
	Mix_Quit();
	good = false;
	logger.LogInfo("Sound uninitialized");
	return true;
}

bool soundMgr::loadSound(const std::string& path, const std::string& ID) 
{
	std::string soundID;

	if(!ID.size())
	{
		soundID = path.substr(path.find_last_of("/\\") + 1, path.find_last_of(".") - path.find_last_of("/\\") - 1);
	}
	else
	{
		soundID = ID;
	}

	if(sounds.find(soundID) != sounds.end())
	{
		logger.LogWarn("Sound ID " + ID + " already taken!");
		return false;
	}

	std::unique_ptr<sound> newS = std::make_unique<sound>();
	if(!newS->load(path))
	{
		logger.LogWarn("Failed to load sound ID " + soundID + " from path " + path + " Mix_error: " + Mix_GetError());
		return false;
	}

	sounds.insert({soundID,std::move(newS)});
	logger.LogInfo("Loaded sound ID " + soundID + " from path " + path);
	return true;
}

bool soundMgr::loadSoundRec(const std::string& path) 
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
	assert(directory);
	if(!directory)
	{
		logger.LogWarn("Failed to open sound directory at " + dirPath);
		return false;
	}

	logger.LogInfo("Loading sound directory at " + dirPath);
	logger.EnterSec();

	// Read directory
	while(entry = readdir(directory))
	{
		std::string entryName = entry->d_name;

		if(entryName != ".." && entryName != ".")
		{
			// Load sound or load another directory
			/// @todo make this better, it just tests for a .extension
			if(entryName[entryName.size() - 4] == '.')
			{
				loadSound(dirPath + entryName);
			}
			else
			{
				loadSoundRec(dirPath + entryName + '/');
			}
		}
	}

	logger.ExitSec();

	// Success
	return true;
}

bool soundMgr::freeSound(const std::string& ID) 
{
	// Get sound
	auto entry = sounds.find(ID);
	if(entry == sounds.end())
	{
		// Failure
		logger.LogWarn("Cannot free sound ID: " + ID + ", does not exist");
		return false;
	}

	// Success
	sounds.erase(entry);
	#ifdef VERBOSE_SOUND
		logger.LogInfo("Freed sound ID: " + ID);
	#endif
	return true;
}


bool soundMgr::pauseAll() 
{
	Mix_Pause(-1);
	#ifdef VERBOSE_SOUND
		logger.LogInfo("Paused all sounds");
	#endif
	return true;
}

bool soundMgr::resumeAll() 
{
	Mix_Resume(-1);
	#ifdef VERBOSE_SOUND
		logger.LogInfo("Resumed all sounds");
	#endif
	return true;
}

bool soundMgr::stopAll(int timeout) 
{
	Mix_ExpireChannel(-1,timeout);
	#ifdef VERBOSE_SOUND
		logger.LogInfo("Stopped or began timeout of all sounds");
	#endif
	return true;
}

bool soundMgr::pause(const std::string& ID) 
{
	// Get sound
	auto entry = sounds.find(ID);
	if(entry == sounds.end())
	{
		// Failure
		logger.LogWarn("Could not find sound to pause, ID:" + ID);
		return false;
	}

	// Success
	Mix_Pause(entry->second->channel);
	#ifdef VERBOSE_SOUND
		logger.LogWarn("Paused sound: " + ID);
	#endif
	return true;
}

bool soundMgr::play(const std::string& ID, int loop, int timeout) 
{
	auto entry = sounds.find(ID);
	if(entry == sounds.end())
	{
		// Failure
		logger.LogWarn("Could not find sound to play, ID:" + ID);
		return false;
	}

	int result = Mix_PlayChannelTimed(-1,(Mix_Chunk*)entry->second->sdl_mixchunk,loop,timeout);
	if(result == -1)
	{
		// Failure
		logger.LogWarn("Failed to play sound ID: " + ID + ", Mix_Error: " + Mix_GetError());
		return false;
	}

	entry->second->channel = result;
	#ifdef VERBOSE_SOUND
		logger.LogInfo("Playing sound ID: " + ID);
	#endif
	return true;
}

bool soundMgr::resume(const std::string& ID) 
{
	// Get sound
	auto entry = sounds.find(ID);
	if(entry == sounds.end())
	{
		// Failure
		logger.LogWarn("Could not find sound to resume, ID:" + ID);
		return false;
	}

	// Success
	Mix_Resume(entry->second->channel);
	#ifdef VERBOSE_SOUND
		logger.LogWarn("Resumed sound: " + ID);
	#endif
	return true;
}

bool soundMgr::stop(const std::string& ID, int timeout) 
{
	// Get sound
	auto entry = sounds.find(ID);
	if(entry == sounds.end())
	{
		// Failure
		logger.LogWarn("Could not find sound to stop, ID:" + ID);
		return false;
	}

	// Success
	Mix_ExpireChannel(entry->second->channel,timeout);
	#ifdef VERBOSE_SOUND
		logger.LogWarn("Stopped sound: " + ID);
	#endif
	return true;
}

// Terminating precompiler directives  ////////////////////////////////////////
