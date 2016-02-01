// Program Information ////////////////////////////////////////////////////////

/**
	@file game.cpp
	@author Max Slater

	@brief "main" for the game code, contains main game loop

	The engine's main function calls into this to run the actual game loop,
	as well as anything that must be done when the game code is loaded or freed

	@version 1.00 (27 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "game_common.h"

/// @todo is it possible for this not inlucde engine_common.h?
#include <engine_state.h>

// Global constant definitions  ///////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

GAME_API void startup(engine_state* engine);
GAME_API bool gameLoop(engine_state* engine);

// Free function implementation  //////////////////////////////////////////////

GAME_API void startup(engine_state* engine) {
	engine->logger.StartLog("GAME");
	engine->logger.LogDefault("Game code initialized");

	engine->sdl.init();
	engine->graphics.init("Game");
	engine->events.init();
}

GAME_API bool gameLoop(engine_state* engine) {
	
	event e;
	while(engine->events.getNextEvent(e))
	{
		if(e.type == EVT_QUIT)
		{
			return false;
		}
	}

	return true;
}

// Terminating precompiler directives  ////////////////////////////////////////
