// Program Information ////////////////////////////////////////////////////////

/**
	@file inputMgr.cpp

	@brief Implementation file for the inputMgr class.

	@author Max Slater

	@version 1.00 (6 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "inputMgr.h"

#include "game_state.h"
#include <engine_state.h>

#include <event/event/event.h>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

inputMgr::inputMgr(game_state* g, engine_state* e)
{
	game = g;
	engine = e;
	inputstate = input_gameplay;
}

inputMgr::~inputMgr()
{

}

void inputMgr::handleEvents()
{
	game->debug.beginProfiledFunc();

	event e;
	while(engine->events.getNextEvent(e))
	{
		if(e.type == EVT_QUIT)
		{
			game->running = false;
		}
		else if(inputstate == input_gameplay)
		{
			handleGameplayEvent(e);
		}
		else if(inputstate == input_debugger)
		{
			handleDebugEvent(e);
		}
	}

	game->debug.endProfiledFunc();
}

void inputMgr::handleGameplayEvent(const event& e)
{
	game->debug.beginProfiledFunc();

	if (e.type == EVT_KEY)
	{
		std::weak_ptr<entity> player = game->map.getPlayerByID("p1");
		std::weak_ptr<component_movement> mov = std::static_pointer_cast<component_movement>(player.lock()->getComponent(ctype_movement).lock());

		if(e.flags & FLAG_KEY_PRESS)
		{
			switch(e.value)
			{
				case KEY_MINUS:
					game->camera.zoom /= 2;
					break;
				case KEY_PLUS:
					game->camera.zoom *= 2;
					break;
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
				case KEY_P:
					game->debug.toggleProfiler();
					break;
				case KEY_O:
					game->render.toggleDebugUI();
					inputstate = input_debugger;
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

	game->debug.endProfiledFunc();
}

void inputMgr::handleDebugEvent(const event& e)
{
	game->debug.beginProfiledFunc();

	if(e.type == EVT_WINDOW)
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
	else if(e.type == EVT_KEY)
	{
		if(e.flags & FLAG_KEY_PRESS)
		{
			switch(e.value)
			{
				case KEY_I:
					engine->time.toggle("sim");
					break;
				case KEY_P:
					game->debug.toggleProfiler();
					break;
				case KEY_O:
					game->render.toggleDebugUI();
					inputstate = input_gameplay;
					break;
				case KEY_UP:
					if(game->debug.selected.lock()->parent.lock()->children.begin()->second == game->debug.selected.lock())
					{
						game->debug.selected = game->debug.selected.lock()->parent;
					}
					else
					{
						game->debug.selected = (game->debug.selected.lock()->parent.lock()->children.find(game->debug.selected.lock()->funcName)--)->second;
					}
					break;
				case KEY_DOWN:
					if(game->debug.selected.lock()->showChildren && game->debug.selected.lock()->children.size())
					{
						game->debug.selected = game->debug.selected.lock()->children.begin()->second;
					}
					// else if((game->debug.selected.lock()->parent.lock()->children.end()--)->second == game->debug.selected.lock())
					// {
						// game->
					// }	
					else
					{
						game->debug.selected = (game->debug.selected.lock()->parent.lock()->children.find(game->debug.selected.lock()->funcName)++)->second;
					}				
					break;
				case KEY_ENTER:
					game->debug.selected.lock()->showChildren = !game->debug.selected.lock()->showChildren;
					break;
			}
		}
	}

	game->debug.endProfiledFunc();
}

// Terminating precompiler directives  ////////////////////////////////////////
