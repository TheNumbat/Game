// Program Information ////////////////////////////////////////////////////////

/**
	@file events.h
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

// Global constant definitions  ///////////////////////////////////////////////

class engine_state;
class game_state;

// Free function prototypes  //////////////////////////////////////////////////

/**
	@brief Handles event loop

	@param[in] engine pointer to engine state
	@param[in] game pointer to game state
*/
void eventLoop(engine_state* engine, game_state* game);

// Free function implementation  //////////////////////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
