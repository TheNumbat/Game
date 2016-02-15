// Program Information ////////////////////////////////////////////////////////

/**
	@file sound.cpp

	@brief Implementaiton file for sound class

	Stores data needed to play a sound

	@author Max Slater

	@version 1.00 (10 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "sound.h"

#include <SDL_mixer.h>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

/**
	@brief sound constructor

	Sets sound to uninititialized
*/
sound::sound()
{
	sdl_mixchunk = NULL;
	channel = -1;
	good = false;
}

/**
	@brief sound destructor

	Frees the sound
*/
sound::~sound()
{
	free();
}

/**
	@brief loads a sound

	Loads sound file using SDL_Mixer.
	Loads WAVE, AIFF, RIFF, OGG, and VOC files

	@param[in] path is the file path of the sound file

	@return success

	@exception SDL_Mixer not initialized, will fail and return false
	@exception Unable to load sound, will fail and return false

	@todo make this log or return some indication of the error
*/
bool sound::load(const std::string& path)
{
	free();

	sdl_mixchunk = Mix_LoadWAV(path.c_str());

	if(sdl_mixchunk) 
	{
		good = true;
		return true;
	} 
	else 
	{
		return false;
	}
}

/**
	@brief Frees sound

	Deallocates sound

	@return success

	@exception if sound is already freed does nothing, returns false
*/
bool sound::free()
{
	if(good)
	{
		Mix_FreeChunk((Mix_Chunk*)sdl_mixchunk);

		good = false;
		return true;
	}
	return false;
}

// Terminating precompiler directives  ////////////////////////////////////////
