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
	eventMgr();
	~eventMgr();

	bool init();
	bool kill();

	bool getNextEvent(event& e);

private:
	event& translateEvent(void* e);
	event& translateKeyboardEvent(void* e);
	event& translateWindowEvent(void* e);
	event& translateMouseEvent(void* e);
	event& translateJoystickEvent(void* e);

	logMgr logger;
	bool good;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
