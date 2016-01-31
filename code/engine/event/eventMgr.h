// Program Information ////////////////////////////////////////////////////////

/**
	@file eventMgr.h

	@brief Declartion file for event manager class.

	Event manager stores and translates events.

	@author Max Slater

	@version 1.00 (28 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include "log/logMgr.h"
#include "event/event.h"

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Manges events by translating the SDL events system

	Used for all input.

	@note Doesn't actually store events, just pulls and translates from the SDL
		  event queue.
*/
class eventMgr
{
public:
	ENGINE_API eventMgr();
	ENGINE_API ~eventMgr();

	ENGINE_API bool init();
	ENGINE_API bool kill();

	ENGINE_API bool getNextEvent(event& e);

private:
	event& translateEvent(void* SDL_ev);
	event& translateKeyboardEvent(void* SDL_ev);
	event& translateWindowEvent(void* SDL_ev);
	event& translateMouseEvent(void* SDL_ev);
	event& translateJoystickEvent(void* SDL_ev);

	logMgr logger;
	bool good;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
