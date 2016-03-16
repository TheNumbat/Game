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
	/**
		@brief sound constructor

		Sets sound to uninititialized
	*/
	sound();

	/**
		@brief sound destructor

		Calls free()
	*/
	~sound();

	/**
		@brief loads a sound

		Loads sound file using SDL_Mixer.
		Loads WAVE, AIFF, RIFF, OGG, and VOC files

		@param[in] path is the file path of the sound file

		@return success

		@exception SDL_Mixer not initialized, will fail and return false
		@exception Unable to load sound, will fail and return false
	*/
	bool load(const std::string& path);

	/**
		@brief Frees sound

		Deallocates sound

		@return success

		@exception if sound is already freed does nothing, returns false
	*/
	bool free();

	/// Pointer to sdl mixer chunk
	void* sdl_mixchunk;
	/// Channel sound is playing on
	int channel;
	/// Sound is good
	bool good;

	friend class soundMgr;

	/// Friendship for smart pointers
	friend class std::default_delete<sound>;
	/// Friendship for smart pointers
	friend std::unique_ptr<sound> std::make_unique<sound>();
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
