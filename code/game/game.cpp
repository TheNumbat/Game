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

GAME_API void startup(engine_state* engine) 
{
	engine->logger.StartLog("GAME");
	engine->logger.LogInfo("Game code initialized");

	engine->sdl.init();
	engine->graphics.init("Game");
	engine->events.init();
	engine->audio.init();
	engine->time.init();

	engine->graphics.loadTextureRec("art");
	engine->audio.loadSoundRec("music");

	engine->time.addTimer("lmao");
	engine->audio.play("song1");

	std::weak_ptr<entity> player1 = engine->map.addPlayer("p1",map_position(0,0,0,0,0,0),0);
	std::weak_ptr<entity> player2 = engine->map.getEntityByUID_SLOW(1);
	assert(player1.lock() == player2.lock());
}

GAME_API bool gameLoop(engine_state* engine) 
{	
	event e;
	while(engine->events.getNextEvent(e))
	{
		if(e.type == EVT_QUIT)
		{
			return false;
		}
		else if (e.type == EVT_KEY)
		{
			if(e.value == KEY_Q)
			{
				engine->time.pause("lmao");
			}
			else if(e.value == KEY_W)
			{
				engine->time.resume("lmao");
			}
		}
	}

	engine->graphics.renderTextureEx("yeti",rect2<int32>(50,50,64,64),rect2<int32>(0,0,32,32),v2<int32>(0,0),0,FLIP_NONE);
	engine->graphics.displayFrame();

	uint64 counter;
	engine->time.get("lmao",counter);
	engine->logger.LogInfo(std::to_string(counter));

	return true;
}

// Terminating precompiler directives  ////////////////////////////////////////
