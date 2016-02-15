// Program Information ////////////////////////////////////////////////////////

/**
	@file rendering.cpp
	@author Max Slater

	@brief does rendering for the game, called in gameLoop

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
	@brief Runs all rendering for the game

	@param[in] engine pointer to the engine state
	@param[in] game pointer to the game state
*/
void render(engine_state* engine, game_state* game);

// Free function implementation  //////////////////////////////////////////////

void render(engine_state* engine, game_state* game)
{
	engine->graphics.renderTexture("yeti",rect2<int32>(50,50,64,64));
	engine->graphics.displayFrame();
}

// Terminating precompiler directives  ////////////////////////////////////////
