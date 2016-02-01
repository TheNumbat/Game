// Program Information ////////////////////////////////////////////////////////

/**
	@file sdlMgr.cpp

	@brief SDL manager just initializes and quits SDL

	@author Max Slater

	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "sdlMgr.h"

#include <SDL.h>
#include <string>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

/**
	@brief sdlMgr constructor

	Sets up log and defaults everything to null and not good
*/
sdlMgr::sdlMgr()
{
	good = false;
	logger.StartLog("SDL");
}

/**
	@brief sdlMgr destructor

	Kills SDL if not already dead
*/
sdlMgr::~sdlMgr()
{
	kill();
}

/**
	@brief initializes SDL

	@exception failed to intialize sdl - return false and do nothing
*/
bool sdlMgr::init()
{
	if( good )
	{
		logger.LogWarn("Trying to reinitialize SDL!");
		return false;
	}

	bool result = SDL_Init(0) == 0;
	assert(result);
	if( !result )
	{
		logger.LogFatal((std::string)"Failed to intialize SDL! SDL Error: " + SDL_GetError());
		return false;
	}

	good = true;
	return true;
}

/**
	@brief initializes SDL

	@exception failed to intialize sdl - return false and do nothing
*/
bool sdlMgr::kill()
{
	if( !good )
	{
		logger.LogWarn("Trying to kill already dead SDL!");
		return false;
	}

	SDL_Quit();
	good = false;
	return true;
}

// Terminating precompiler directives  ////////////////////////////////////////
