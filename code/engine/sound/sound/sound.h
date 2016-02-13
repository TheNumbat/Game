// Program Information ////////////////////////////////////////////////////////

/**
	@file sound.h

	@brief Declartion file for sound class

	Sound stores all information needed to play a sound

	@author Max Slater

	@version 1.00 (10 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include <string>
#include <memory>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Abtracts an SDL_mixer sound 

	Holds data for a sound.

	Used by soundMgr.
*/
class sound
{
	sound();
	~sound();

	bool load(const std::string& path);
	bool free();

	void* sdl_mixchunk;
	int channel; /// used by soundMgr
	bool good;

	friend class soundMgr;

	/// @note 
	friend class std::default_delete<sound>;
	friend std::unique_ptr<sound> std::make_unique<sound>();
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
