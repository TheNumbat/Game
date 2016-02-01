// Program Information ////////////////////////////////////////////////////////

/**
	@file engine_state.h

	@brief Declartion file for state class.

	state does not do anything in of itself, it just holds together all the
	engine components for use in the game logic code.

	@author Max Slater

	@version 1.00 (30 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include "event/eventMgr.h"
#include "log/logMgr.h"
#include "graphic/graphicMgr.h"
#include "sdl/sdlMgr.h"


// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Collects engine modules together for use in the game code.

	Used so the game logic can call into the engine

	@note should only be created once, in the main function of the engine.
*/
struct engine_state
{
	// MUST INIT SDL FIRST
	sdlMgr sdl;
	
	logMgr logger;
	eventMgr events;
	graphicMgr graphics;
	// soundMgr audio;
	// threadMgr threads;
	// timeMgr timing;
	// mapMgr map;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
