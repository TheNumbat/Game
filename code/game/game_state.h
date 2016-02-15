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
#include <log/logMgr.h>

// Typedefs ///////////////////////////////////////////////////////////////////

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Describes the overall game state -- used to store any information
	used within the game logic but not the engine.

	@note should only be created once, in the main function of the game.
*/
struct game_state
{
	game_state() : cam() {};
	~game_state() {};

	camera cam;
	logMgr logger;
	bool running;
};

// Free function prototypes  //////////////////////////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
