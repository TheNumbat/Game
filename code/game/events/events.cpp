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

#include "events.h"

#include "game_state/game_state.h"
#include <engine_state.h>

// Global constant definitions  ///////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

void eventLoop(engine_state* engine, game_state* game)
{
	event e;
	while(engine->events.getNextEvent(e))
	{
		std::weak_ptr<entity> player = game->map.getPlayerByID("p1");
		std::weak_ptr<component_movement> mov = std::static_pointer_cast<component_movement>(player.lock()->getComponent(ctype_movement).lock());

		if(e.type == EVT_QUIT)
		{
			game->running = false;
		}
		else if(e.type == EVT_WINDOW)
		{
			if(e.flags & FLAG_WINDOW_RESIZED)
			{
				uint32 w = e.value, h = e.value >> 32;
				game->logger.LogInfo("New screen w: " + std::to_string(w) + " h: " + std::to_string(h));
			}
		}
		else if(e.type == EVT_MOUSE)
		{
			if(e.flags & FLAG_MOUSE_PRESS)
			{
				uint32 x = e.value, y = e.value >> 32;
				game->logger.LogInfo("Mouse x: " + std::to_string(x) + " y: " + std::to_string(y));
			}
		}
		else if (e.type == EVT_KEY)
		{
			if(e.value == KEY_MINUS && e.flags & FLAG_KEY_PRESS)
			{
				game->cam.zoom /= 2;
			}
			else if(e.value == KEY_PLUS && e.flags & FLAG_KEY_PRESS)
			{
				game->cam.zoom *= 2;
			}

			if(e.flags & FLAG_KEY_PRESS)
			{
				switch(e.value)
				{
					case KEY_LEFT:
						mov.lock()->velocity = v2<real32>(-5,mov.lock()->velocity.y);
						break;
					case KEY_RIGHT:
						mov.lock()->velocity = v2<real32>(5,mov.lock()->velocity.y);
						break;
					case KEY_UP:
						mov.lock()->velocity = v2<real32>(mov.lock()->velocity.x,-5);
						break;
					case KEY_DOWN:
						mov.lock()->velocity = v2<real32>(mov.lock()->velocity.x,5);
						break;
				}
			}
			else if(e.flags & FLAG_KEY_RELEASE)
			{
				switch(e.value)
				{
					case KEY_LEFT:
					case KEY_RIGHT:
						mov.lock()->velocity = v2<real32>(0,mov.lock()->velocity.y);	
						break;
					case KEY_UP:
					case KEY_DOWN:
						mov.lock()->velocity = v2<real32>(mov.lock()->velocity.x,0);
						break;
				}
			}
		}
	}
}

// Terminating precompiler directives  ////////////////////////////////////////
