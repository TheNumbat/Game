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

sound::sound()
{
	sdl_mixchunk = NULL;
	channel = -1;
	good = false;
}

sound::~sound()
{
	free();
}

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
