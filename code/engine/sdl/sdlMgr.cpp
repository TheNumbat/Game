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

sdlMgr::sdlMgr()
{
	good = false;
	logger.StartLog("SDL");
}

sdlMgr::~sdlMgr()
{
	kill();
}

bool sdlMgr::init()
{
	if(good)
	{
		logger.LogWarn("Trying to reinitialize SDL!");
		return false;
	}

	// Init SDl
	bool result = SDL_Init(0) == 0;
	assert(result);
	if(!result)
	{
		logger.LogFatal((std::string)"Failed to intialize SDL! SDL Error: " + SDL_GetError());
		return false;
	}

	logger.LogInfo("SDL initialized");

	// Success
	good = true;
	return true;
}

bool sdlMgr::kill()
{
	if(!good)
	{
		logger.LogWarn("Trying to kill already dead SDL!");
		return false;
	}

	// Quit
	SDL_Quit();

	logger.LogInfo("SDL uninitialized");

	// Success
	good = false;
	return true;
}

// Terminating precompiler directives  ////////////////////////////////////////
