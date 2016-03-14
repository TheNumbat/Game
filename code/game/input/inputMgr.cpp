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

	event* e;
	do
	{
		e = engine->events.getNextEvent();

		if(e->type == EVT_QUIT)
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

		delete e;

	} while(e->type != EVT_BAD);

	game->debug.endProfiledFunc();
}

void inputMgr::handleTextEvent(event* e, const std::string& exclude)
{
	text_event* te = dynamic_cast<text_event*>(e);
	if(te)
	{
		if(te->text.find_first_of(exclude) != std::string::npos) return;
		inputStr += te->text;
	}
}

void inputMgr::handleGameplayEvent(event* e)
{
	game->debug.beginProfiledFunc();

	if (e->type == EVT_KEY)
	{
		std::weak_ptr<entity> player = game->map.getPlayerByID("p1");
		std::weak_ptr<component_movement> mov = std::static_pointer_cast<component_movement>(player.lock()->getComponent(ctype_movement).lock());

		if(e->flags & FLAG_KEY_PRESS)
		{
			switch(e->value)
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
					game->debug.renderDebugUI = !game->debug.renderDebugUI;
					inputstate = input_debugger;
					break;
			}
		}
		else if(e->flags & FLAG_KEY_RELEASE)
		{
			switch(e->value)
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

void inputMgr::handleDebugEvent(event* e)
{
	game->debug.beginProfiledFunc();

	if(e->type == EVT_WINDOW)
	{
		if(e->flags & FLAG_WINDOW_RESIZED)
		{
			uint32 w = e->value, h = e->value >> 32;
			game->logger.LogInfo("New screen w: " + std::to_string(w) + " h: " + std::to_string(h));
		}
	}
	else if(e->type == EVT_MOUSE)
	{
		if(e->flags & FLAG_MOUSE_PRESS)
		{
			uint32 x = e->value, y = e->value >> 32;
			game->logger.LogInfo("Mouse x: " + std::to_string(x) + " y: " + std::to_string(y));
		}
	}
	else if(e->type == EVT_KEY)
	{
		if(e->flags & FLAG_KEY_PRESS)
		{
			switch(e->value)
			{
				case KEY_UP:
					if(game->debug.selected.lock() != game->debug.profileHead)
					{
						if(game->debug.selected.lock()->parent.lock()->children.begin()->second == game->debug.selected.lock())
						{
							game->debug.selected = game->debug.selected.lock()->parent;
						}
						else if(game->debug.selected.lock() != game->debug.selected.lock()->parent.lock()->children.begin()->second)
						{
							auto temp = std::prev(game->debug.selected.lock()->parent.lock()->children.find(game->debug.selected.lock()->funcName));	
							if(temp->second->showChildren && temp->second->children.size())
							{
								game->debug.selected = std::prev(temp->second->children.end())->second;
							}

							else if(std::distance(game->debug.selected.lock()->parent.lock()->children.begin(),game->debug.selected.lock()->parent.lock()->children.find(game->debug.selected.lock()->funcName)) > 0)
							{
								game->debug.selected = std::prev(game->debug.selected.lock()->parent.lock()->children.find(game->debug.selected.lock()->funcName))->second;
							}
						}
					}
					else
					{
						while(game->debug.selected.lock()->showChildren && game->debug.selected.lock()->children.size())
						{
							game->debug.selected = std::prev(game->debug.selected.lock()->children.end())->second;
						}
					}
					break;
				case KEY_DOWN:
					if(game->debug.selected.lock()->showChildren && game->debug.selected.lock()->children.size())
					{
						game->debug.selected = game->debug.selected.lock()->children.begin()->second;
					}
					else if(game->debug.selected.lock() != game->debug.profileHead)
					{
						if(std::distance(game->debug.selected.lock()->parent.lock()->children.find(game->debug.selected.lock()->funcName),game->debug.selected.lock()->parent.lock()->children.end()) > 1)
						{
							game->debug.selected = std::next(game->debug.selected.lock()->parent.lock()->children.find(game->debug.selected.lock()->funcName))->second;
						}
						else
						{
							bool found = false;
							while(!found)
							{
								game->debug.selected = game->debug.selected.lock()->parent;
								if(game->debug.selected.lock()->parent.expired())
								{
									break;
								}
								if(std::distance(game->debug.selected.lock()->parent.lock()->children.find(game->debug.selected.lock()->funcName),game->debug.selected.lock()->parent.lock()->children.end()) > 1)
								{
									game->debug.selected = std::next(game->debug.selected.lock()->parent.lock()->children.find(game->debug.selected.lock()->funcName))->second;
									found = true;
								}
							}
						}
					}
					break;
				case KEY_ENTER:
					if(game->debug.inputConsole)
					{
						if(inputStr == "reload")
						{
							game->debug.loadConsoleFuncs();
						}
						else
						{
							game->debug.callConsoleFunc(inputStr);
							game->debug.inputConsole = false;
						}
						inputStr = "";
					}
					else
					{
						game->debug.selected.lock()->showChildren = !game->debug.selected.lock()->showChildren;
					}
					break;
				case KEY_GRAVE:
					game->debug.inputConsole = !game->debug.inputConsole;
					break;
			}

			if(!game->debug.inputConsole)
			{
				switch(e->value)
				{
					case KEY_P:
						game->debug.toggleProfiler();
						break;
					case KEY_O:
						game->debug.renderDebugUI = !game->debug.renderDebugUI;
						inputstate = input_gameplay;
						break;
				}
			}
			else if(game->debug.inputConsole)
			{
				if(e->value == KEY_BACKSPACE && inputStr.length())
				{
					inputStr.pop_back();
				}
				else if(e->flags & FLAG_KEY_CTRL)
				{
					switch(e->value)
					{
						case KEY_V:
							engine->events.paste(inputStr);
							break;
						case KEY_C:
							engine->events.copy(inputStr);
							break;
					}
				}
			}
		}
	}

	if(game->debug.inputConsole && e->type == EVT_TEXT)
	{	
		handleTextEvent(e,"`");
	}

	game->debug.endProfiledFunc();
}

// Terminating precompiler directives  ////////////////////////////////////////
