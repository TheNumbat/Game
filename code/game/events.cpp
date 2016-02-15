// Program Information ////////////////////////////////////////////////////////

/**
	@file events.cpp
	@author Max Slater

	@brief manages the event system for the game, contains the event loop

	Called in gameLoop()

	@version 1.00 (15 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "game_common.h"
#include <engine_state.h>

// Global constant definitions  ///////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

/**
	@brief Runs the event loop, managing all input and other events for the game

	@param[in] engine pointer to the engine state
	@param[in] game pointer to the game state
*/
void eventLoop(engine_state* engine, game_state* game);

// Free function implementation  //////////////////////////////////////////////

void eventLoop(engine_state* engine, game_state* game)
{
	event e;
	while(engine->events.getNextEvent(e))
	{
		if(e.type == EVT_QUIT)
		{
			game->running = false;
		}
		else if (e.type == EVT_KEY)
		{
			
		}
	}
}

// Terminating precompiler directives  ////////////////////////////////////////
