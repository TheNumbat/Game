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

#include "events/events.h"
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

	engine->logger.StartLog("ENGINE");

	engine->sdl.init();
	engine->graphics.init("Game");
	engine->events.init();
	engine->audio.init();
	engine->time.init();

	engine->graphics.loadTextureRec("debug");
	engine->graphics.loadTexture("art/entities/dankdude_front.png","dankdude");
	engine->graphics.loadTexture("art/entities/yeti.png","yeti");

	engine->graphics.loadFont("fonts/aubrey.ttf","aubrey_150",150);
	engine->graphics.loadFont("fonts/aubrey.ttf","aubrey_24",24);

	engine->time.addTimer("sim");

	std::weak_ptr<entity> player = game->map.addPlayer("p1",map_position(0,0,0,3,3,0),0);
	std::weak_ptr<entity> test = game->map.addEntity(map_position(0,0,0,3,3,0),0);

	std::weak_ptr<component_movement> mov = std::static_pointer_cast<component_movement>(player.lock()->addComponent(ctype_movement).lock());
	mov = std::static_pointer_cast<component_movement>(test.lock()->addComponent(ctype_movement).lock());
	mov.lock()->velocity = v2<real32>(3,0);

	std::weak_ptr<component_texture> texture = std::static_pointer_cast<component_texture>(player.lock()->addComponent(ctype_texture).lock());
	texture.lock()->addTexture("main","dankdude",v2<real32>(-0.5,-0.5),v2<real32>(1,1));
	texture.lock()->forceTop = true;

	std::weak_ptr<component_text_texture> text = std::static_pointer_cast<component_text_texture>(player.lock()->addComponent(ctype_text_texture).lock());
	text.lock()->addText("main","aubrey_24","ayy lmao",v2<real32>(-0.5,-0.75),v2<real32>(1,0.25));

	std::weak_ptr<component_texture> testtexture = std::static_pointer_cast<component_texture>(test.lock()->addComponent(ctype_texture).lock());
	testtexture.lock()->addTexture("main","yeti",v2<real32>(-0.5,-0.5),v2<real32>(1,1),blend_additive);

	game->cam.setFollowing(player);

	game->running = true;
	return game;
}

GAME_API bool gameLoop(engine_state* engine, game_state* game)
{	
	eventLoop(engine,game);
	simulate(engine,game,"sim");
	render(engine,game);

	return game->running;
}

GAME_API void shutdown(engine_state* engine, game_state* game)
{
	delete game;
}

// Terminating precompiler directives  ////////////////////////////////////////
