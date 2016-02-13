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

/**
	@brief soundMgr constructur

	Defaults values and starts log
	Call init() to set up sound
*/
soundMgr::soundMgr() 
{
	good = false;
	logger.StartLog("SOUND");
}

/**
	@brief soundMgr denstructur

	Calls kill()
*/
soundMgr::~soundMgr() 
{
	kill();
}

/**
	@brief initializes sound

	Calls Mix_OpenAdio

	@return success

	@exception already initialized, returns false
	@exception failed to open audio, returns false
*/
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

/**
	@brief deinitializes sound

	Calls Mix_Quit

	@return success

	@exception sound already deinitialized, returns false
*/
bool soundMgr::kill() 
{
	if(!good)
	{
		logger.LogWarn("Cannot re-destroy sound!");
		return false;
	}

	// Success
	Mix_Quit();
	good = false;
	logger.LogInfo("Sound uninitialized");
	return true;
}

/**
	@brief Loads a sound from a file

	@param[in] path to sound file to load
	@param[in] repeat_ number of times to repeat sound (when played), 
			   or -1 to loop until stopped.
	@param[in] ID of sound

	@return success

	@exception failed to load sound, returns false
*/
bool soundMgr::loadSound(const std::string& path, const std::string& ID) 
{
	if(sounds.find(ID) != sounds.end())
	{
		logger.LogWarn("Sound ID " + ID + " already taken!");
		return false;
	}

	std::string soundID;

	if(!ID.size())
	{
		soundID = path.substr(path.find_last_of("/\\") + 1, path.find_last_of(".") - path.find_last_of("/\\") - 1);
	}
	else
	{
		soundID = ID;
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

/**
	@brief Loads all image files from a folder (and subfolders) into textute map

	Will just use file name for texture IDs

	@param[in] path file path of image file

	@note uses dirent.h

	@return success -- always true
*/
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

/**
	@brief unloads a sound

	@param[in] ID of sound to unload

	@return sucess

	@exception sound not found, returns false
*/
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
	return true;
}


/**
	@brief Pauses all currently playing sounds

	@return success -- always true
*/
bool soundMgr::pauseAll() 
{
	Mix_Pause(-1);
	return true;
}

/**
	@brief Resumes all currently paused or playing channels

	@return success -- always true
*/
bool soundMgr::resumeAll() 
{
	Mix_Resume(-1);
	return true;
}

/**
	@brief stops all sounds playing, 

	@param[in] timeout how long to wait in milliseconds until halting all chanells.
			   Default is instant.

	@return success -- always true
*/
bool soundMgr::stopAll(int timeout) 
{
	Mix_ExpireChannel(-1,timeout);
	return true;
}

/**
	@brief pauses a sound

	@param[in] ID of sound to pause

	@return success

	@exception could not find sound, returns false
	@exception could not play sound, returns false
*/
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
	logger.LogWarn("Paused sound: " + ID);
	return true;
}

/**
	@brief Plays a loaded sound

	@param[in] ID of sound to play
	@param[in] number of times to loop. Default is 0 = play once,
			   -1 to loop unitl stopped
	@param[in] timeout time to stop playing sound in milliseconds.
			   Default is -1 = play until finished

	@return success

	@exception couldn't find sound -- returns false
	@exception failed to play sound -- returns false
*/
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
	logger.LogInfo("Playing sound ID: " + ID);
	return true;
}

/**
	@brief Resumes a paused sound

	@param[in] ID of sound to resume

	@return success

	@exception could not find sound, returns false
*/
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
	logger.LogWarn("Resumed sound: " + ID);
	return true;
}

/**
	@brief Stops a playing/paused sound

	@param[in] ID of sound to stop
	@param[in] timeout how long to wait in milliseconds until halting the chanell.
			   Default is instant.

	@return success

	@exception could not find sound
*/
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
	logger.LogWarn("Stopped sound: " + ID);
	return true;
}

// Terminating precompiler directives  ////////////////////////////////////////
