// Program Information ////////////////////////////////////////////////////////

/**
	@file sdlMgr.h

	@brief Header file for the sdlMgr class

	sdlMgr simply manages initializing and destroying SDL

	@author Max Slater

	@version 1.00 (15 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include "log/logMgr.h"

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Manages opening/closing SDL

	SDL Must be initalized before any other subsystems
*/
class sdlMgr
{
public:
	/**
		@brief sdlMgr constructor

		Sets up log and defaults everything to null and not good
	*/
	ENGINE_API sdlMgr();

	/**
		@brief sdlMgr destructor

		calls kill()
	*/
	ENGINE_API ~sdlMgr();

	/**
		@brief initializes SDL

		@exception failed to intialize sdl - return false and do nothing
	*/
	ENGINE_API bool init();

	/**
		@brief deinitializes SDL

		Quits everything, calling SDL_Quit
	*/
	ENGINE_API bool kill();

private:
	/// Logger
	logMgr logger;
	/// SDL is good
	bool good;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
