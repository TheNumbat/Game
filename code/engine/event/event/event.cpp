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

event::event(uint8 t, uint32 v, uint32 f)
{
	type = t;
	value = v;
	flags = f;
}

event::~event()
{
	
}

event::operator bool() const
{
	return type != EVT_BAD;
}

event& event::operator=(const event& src)
{
	type = src.type;
	value = src.value;
	flags = src.flags;

	return *this;
}

// Terminating precompiler directives  ////////////////////////////////////////
