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

#include "event/event.h"
#include "render/render.h"
#include "sim/sim.h"

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
	@brief Ends the game, deletes the state
	
	Initializes all engine components and creates a game state

	@param[in] engine pointer to the engine state
	@param[in] game pointer to the game state
*/
GAME_API void shutdown(engine_state* engine, game_state* game);

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
	engine->graphics.loadTextureRec("debug");
	engine->audio.loadSoundRec("music");
	engine->time.addTimer("sim");

	std::weak_ptr<entity> player = game->map.addPlayer("p1",map_position(0,0,0,3,3,0),0);

	std::weak_ptr<component_movement> mov = std::static_pointer_cast<component_movement>(player.lock()->addComponent(ctype_movement).lock());
	mov.lock()->velocity = v2<real32>(10,0);

	std::weak_ptr<component_texture> texture = std::static_pointer_cast<component_texture>(player.lock()->addComponent(ctype_texture).lock());
	texture.lock()->addTexture("dankdude_front",v2<real32>(-0.5,-0.5),v2<real32>(1,1));

	game->cam.setFollowing(player.lock());

	game->sim.exit = false;
	game->sim.game = game;
	game->sim.engine = engine;
	game->sim.timerID = "sim";
	engine->thread.add("sim1",&simulate,&game->sim);

	game->running = true;
	return game;
}

GAME_API bool gameLoop(engine_state* engine, game_state* game)
{	
	eventLoop(engine,game);
	renderMap(engine,game);

	engine->graphics.displayFrame();

	return game->running;
}

GAME_API void shutdown(engine_state* engine, game_state* game)
{
	game->sim.exit = true;
	engine->thread.wait("sim1");
	delete game;
}

// Terminating precompiler directives  ////////////////////////////////////////
