// Program Information ////////////////////////////////////////////////////////

/**
	@file startup.h
	@author Max Slater

	@brief Sets up the game and engine, loads files, etc

	@version 1.00 (18 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "game_common.h"

// Global constant definitions  ///////////////////////////////////////////////

class engine_state;
class game_state;

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

/**
	@brief Sets up the game and engine, loads files, etc

	@param[in] engine pointer to engine state
	@param[in] game pointer to game state
*/
void gamestart(engine_state* engine, game_state* game);

// Free function implementation  //////////////////////////////////////////////
