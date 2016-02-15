// Program Information ////////////////////////////////////////////////////////

/**
	@file game_main.cpp
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

#include "game_common.h"
#include <engine_state.h>

#include "events.cpp"
#include "rendering.cpp"

// Global constant definitions  ///////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

/**
	@brief Starts up the game, initializes the engine
	
	Initializes all engine components and creates a game state

	@param[in] engine pointer to the engine state
	
	@return pointer to the new game state
*/
GAME_API game_state* startup(engine_state* engine);

/**
	@brief Runs a frame, it's the main game loop
	
	Runs everything.

	@param[in] engine pointer to the engine state
	@param[in] game pointer to the game state
	
	@return whether or not to continue
*/
GAME_API bool gameLoop(engine_state* engine, game_state* game);

// Free function implementation  //////////////////////////////////////////////

GAME_API game_state* startup(engine_state* engine) 
{
	game_state* game = new game_state;
	game->logger.StartLog("GAME");
	game->logger.LogInfo("Game code initialized");

	engine->sdl.init();
	engine->graphics.init("Game");
	engine->events.init();
	engine->audio.init();
	engine->time.init();

	engine->graphics.loadTextureRec("art");
	engine->audio.loadSoundRec("music");

	game->running = true;
	return game;
}

GAME_API bool gameLoop(engine_state* engine, game_state* game) 
{	
	eventLoop(engine,game);
	render(engine,game);

	return game->running;
}

// Terminating precompiler directives  ////////////////////////////////////////
