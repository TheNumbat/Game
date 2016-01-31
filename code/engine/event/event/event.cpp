// Program Information ////////////////////////////////////////////////////////

/**
	@file event.cpp

	@brief Implementation of event class

	Event describes a single input event for the game, including keyboard,
	mouse, window, and windows events. Used by event manager.

	@author Max Slater
	
	@version 1.00 (28 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "event.h"

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

/**
	@brief event default/parameterized constructor.

	@param[in] t event type. See event type constants in event.h
	@param[in] v event value. See event type constants in event.h
	@param[in] f event flags. See event type constants in event.h
*/
event::event(uint8 t, uint32 v, uint32 f)
{
	type = t;
	value = v;
	flags = f;
}

/**
	@brief event destructor (no detailed docs)
*/
event::~event()
{
	
}

/**
	@brief tests if event is valid

	Specifically, tests if the type is not EVT_BAD
*/
event::operator bool()
{
	return type != EVT_BAD;
}

// Terminating precompiler directives  ////////////////////////////////////////
