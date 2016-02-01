// Program Information ////////////////////////////////////////////////////////

/**
	@file graphicMgr.h

	@brief Implementaiton file for graphicMgr class

	graphicMgr manages all things graphical

	@author Max Slater

	@version 1.00 (31 Jan 2016)
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
	ENGINE_API sdlMgr();
	ENGINE_API ~sdlMgr();

	ENGINE_API bool init();
	ENGINE_API bool kill();

private:
	logMgr logger;
	bool good;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
