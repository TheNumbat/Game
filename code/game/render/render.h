// Program Information ////////////////////////////////////////////////////////

/**
	@file render.h
	@author Max Slater

	@brief does rendering for the game, called in gameLoop

	@version 1.00 (15 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

/// Rendering macros
#define PIXELS_TO_METERS (1.0f / METERS_TO_PIXELS)
#define CHUNK_SIZE_PIXELS (CHUNK_SIZE_METERS * METERS_TO_PIXELS)

// Header files ///////////////////////////////////////////////////////////////

#include "game_common.h"

// Global constant definitions  ///////////////////////////////////////////////

class game_state;
class engine_state;

const uint32 TEXT_LAYER = (uint32)-1;

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

/**
	@brief Renders the game 

	@param[in] engine pointer to the engine state
	@param[in] game pointer to the game state
*/
void render(engine_state* engine, game_state* game);

///////////////////////////////////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
