// Program Information ////////////////////////////////////////////////////////

/**
	@file game_state.h
	@author Max Slater

	@brief Describes the game state

	@version 1.00 (28 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

#include "camera/camera.h"
#include "map/mapMgr.h"

#include <log/logMgr.h>

// Typedefs ///////////////////////////////////////////////////////////////////

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

struct debug_info
{
	uint64 perfCountFreq;
};

/**
	@brief Describes the overall game state -- used to store any information
	used within the game logic but not the engine.

	@note should only be created once, in the main function of the game.
*/
struct game_state
{
	game_state() 
	{
		running = false;
		showDebugUI = false;
	};
	~game_state() {};

	debug_info debug;
	camera cam;
	mapMgr map;
	logMgr logger;
	bool running;
	bool showDebugUI;
};

// Free function prototypes  //////////////////////////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
