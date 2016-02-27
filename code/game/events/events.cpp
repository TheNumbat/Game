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

			//If the player is holding two directions at once, make them go diagonally
			else if(e.value == KEY_LEFT && e.flags & FLAG_KEY_PRESS)
			{
				if(mov.lock()->velocity.x == 0){
					mov.lock()->velocity = v2<real32>(-2.5,mov.lock()->velocity.y/2);
				}
				else{
					mov.lock()->velocity = v2<real32>(-5,0);
				}
			}
			else if(e.value == KEY_RIGHT && e.flags & FLAG_KEY_PRESS)
			{
				if(mov.lock()->velocity.x == 0){
					mov.lock()->velocity = v2<real32>(2.5,mov.lock()->velocity.y/2);
				}
				else{
					mov.lock()->velocity = v2<real32>(5,0);
				}
			}
			else if(e.value == KEY_UP && e.flags & FLAG_KEY_PRESS)
			{
				if(mov.lock()->velocity.y == 0){
					mov.lock()->velocity = v2<real32>(mov.lock()->velocity.x/2,-2.5);
				}
				else{
					mov.lock()->velocity = v2<real32>(0,-5);
				}
			}
			else if(e.value == KEY_DOWN && e.flags & FLAG_KEY_PRESS)
			{
				if(mov.lock()->velocity.y == 0){
					mov.lock()->velocity = v2<real32>(mov.lock()->velocity.x/2,2.5);
				}
				else{
					mov.lock()->velocity = v2<real32>(0,5);
				}
			}
			/*else if(e.flags & FLAG_KEY_RELEASE)
			{
				mov.lock()->velocity = v2<real32>(0,0);
			}*/
		}
	}
}

// Terminating precompiler directives  ////////////////////////////////////////
