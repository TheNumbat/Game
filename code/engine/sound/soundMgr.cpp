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
	sdl_mixer = NULL;
	good = false;
	loger.StartLog(false,"SOUND");
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
		logger.LogFatal("Failed to initalize sound SDL_Mixer Error: " + Mix_GetError());
		return false;
	}
	
	// Success
	good = true;
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
	if(good)
	{
		logger.LogWarn("Cannot re-destroy sound!");
		return false;
	}

	// Success
	Mix_Quit();
	good = false;
	return true;
}

/**
	@brief

	@param[in]

	@return
*/
bool soundMgr::loadSound(const std::string& path, int repeat_, const std::string& ID) 
{

}

/**
	@brief

	@param[in]

	@return
*/
bool soundMgr::loadSoundRec(const std::string& path) 
{

}

/**
	@brief

	@param[in]

	@return
*/
bool soundMgr::freeSound(const std::string& ID) 
{

}


/**
	@brief

	@param[in]

	@return
*/
bool soundMgr::pauseAll() 
{

}

/**
	@brief

	@param[in]

	@return
*/
bool soundMgr::playAll() 
{
	
}

/**
	@brief

	@param[in]

	@return
*/
bool soundMgr::playpauseAll() 
{

}

/**
	@brief

	@param[in]

	@return
*/
bool soundMgr::stopAll() 
{
	
}

/**
	@brief

	@param[in]

	@return
*/
bool soundMgr::pause(const std::string& ID) 
{
	
}

/**
	@brief

	@param[in]

	@return
*/
bool soundMgr::play(const std::string& ID) 
{
	
}

/**
	@brief

	@param[in]

	@return
*/
bool soundMgr::playpause(const std::string& ID) 
{
	
}

/**
	@brief

	@param[in]

	@return
*/
bool soundMgr::stop(const std::string& ID) 
{
	
}

// Terminating precompiler directives  ////////////////////////////////////////
