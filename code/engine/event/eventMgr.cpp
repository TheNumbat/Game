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

#include "eventMgr.h"
#include <SDL.h>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

/**
	@brief eventMgr constructor

	Sets up logging.
*/
eventMgr::eventMgr()
{
	good = false;
	logger.StartLog("EVENTS");
}

/**
	@brief eventMgr destructor

	Does literally nothing.
*/
eventMgr::~eventMgr()
{

}

/**
	@brief initializes events system

	Sets up SDL events 
*/
bool eventMgr::init()
{
	if( !good )
	{
		int result = SDL_InitSubSystem(SDL_INIT_EVENTS);
		assert(result == 0);
		if( result < 0 )
		{
			logger.LogFatal("Failed to initialize SDL events");
			return false;
		}

		good = true;
		return true;
	}
	else
	{
		logger.LogWarn("Trying to reinitialize events");
	}
}

/**
	@brief kills events system

	Sets up SDL events 
*/
bool eventMgr::kill()
{
	if( good )
	{
		SDL_QuitSubSystem(SDL_INIT_EVENTS);

		good = false;

		return true;
	}
	else
	{
		logger.LogWarn("Trying to kill uninitialized events");
	}
}

/**
	@brief takes and translates next event from the queue

	Takes an event from the SDL event queue, translates and 
	returns

	@param[out] e translated event. EVT_BAD if no events were available
*/
bool eventMgr::getNextEvent(event& e)
{
	if( good )
	{
		SDL_Event sdlE;

		if( SDL_PollEvent(&sdlE) == 0 )
		{
			return event(EVT_BAD);
		}
		else
		{
			return translateEvent(&sdlE);
		}
	}
	else
	{
		logger.LogWarn( "Uniniitialized -- trying to get next event" );
	}
}

/**
	@brief translates SDL_Event to event

	@param[in] e SDL_Event to translate

	@return translated event
*/
event& eventMgr::translateEvent(void* e)
{
	switch(((SDL_EVENT)e).type)
	{
		case SDL_QUIT:
			return event(EVT_QUIT);
		case SDL_WINDOWEVENT:
			return translateWindowEvent(e);
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			return translateKeyboardEvent(e);
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEWHEEL:
			return translateMouseEvent(e);
		case SDL_JOYAXISMOTION:
		case SDL_JOYBALLMOTION:
		case SDL_JOYHATMOTION:
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
		case SDL_JOYDEVICEADDED:
		case SDL_JOYDEVICEREMOVED:
			return trandlateJoystickEvent(e);
	}
}

/**
	@brief translates keyboard SDL_Event to event

	@param[in] e SDL_Event to translate

	@return translated event
*/
event& eventMgr::translateEvent(void* e)
{
	
}

// Terminating precompiler directives  ////////////////////////////////////////
