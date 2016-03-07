// Program Information ////////////////////////////////////////////////////////

/**
	@file dll_interface.cpp
	@author Max Slater

	@brief "main" for the game code, contains main game loop, startup, and reload
		   functions

	The engine's main function calls into this to run the actual game loop,
	as well as anything that must be done when the game code is loaded or freed

	@version 1.00 (27 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "game_state.h"

// Global constant definitions  ///////////////////////////////////////////////

class engine_state;

// Free function prototypes  //////////////////////////////////////////////////

/**
	@brief Starts up the game, initializes the engine
	
	Initializes all engine components and creates a game state

	@param[in] engine pointer to the engine state
	
	@return pointer to the new game state
*/
GAME_API game_state* startup(engine_state* engine) 
{
	return new game_state(engine);
}

/**
	@brief Runs the game loop
	
	Runs the game loop, simply an exported wrapper function

	@param[in] game pointer to the game state

	@return whether or not to continue running the game loop (otherwise shutdown())
*/
GAME_API bool gameLoop(game_state* game)
{	
	return game->gameLoop();
}

/**
	@brief Shuts down the game, deletes memory, deinitalizes everything
	
	Destroys the game, simply an exported wrapper function

	@param[in] game pointer to the game state
*/
GAME_API void shutdown(game_state* game)
{
	delete game;
}

// Free function implementation  //////////////////////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
