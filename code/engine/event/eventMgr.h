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
	/**
		@brief eventMgr constructor

		Sets up logging.
	*/
	ENGINE_API eventMgr();

	/**
		@brief eventMgr destructor

		Calls kill()
	*/
	ENGINE_API ~eventMgr();

	/**
		@brief initializes events system

		Sets up SDL events 
	*/
	ENGINE_API bool init();

	/**
		@brief kills events system

		Ends SDL Events system
	*/
	ENGINE_API bool kill();

	/**
		@brief takes and translates next event from the queue

		Takes an event from the SDL event queue, translates and 
		returns

		@param[out] e translated event. EVT_BAD if no events were available
	*/
	ENGINE_API event* getNextEvent();


	/**
		@brief Checks if a specific key is pressed

		@param[in] keycode of key to check
	*/
	ENGINE_API bool keyPressed(uint64 keycode);

	ENGINE_API bool paste(std::string& text);
	ENGINE_API bool copy(const std::string& text);

private:
	/**
		@brief translates SDL_Event to event using specific translate functions

		@param[in] SDL_ev SDL_Event to translate

		@return translated event from individualed translate functions
	*/
	event* translateEvent(void* SDL_ev);

	/**
		@brief translates window SDL_Event to event

		@param[in] SDL_ev SDL_Event to translate

		@return translated event
	*/
	event* translateKeyboardEvent(void* SDL_ev);

	/**
		@brief translates joystick SDL_Event to event

		@param[in] SDL_ev SDL_Event to translate

		@return translated event
	*/
	event* translateWindowEvent(void* SDL_ev);

	/**
		@brief translates mouse SDL_Event to event

		@param[in] SDL_ev SDL_Event to translate

		@return translated event

		@note the mouse x and y relative to the window are returned in the event value
	*/
	event* translateMouseEvent(void* SDL_ev);

	/**
		@brief translates keyboard SDL_Event to event

		@param[in] SDL_ev SDL_Event to translate

		@return translated event

		@todo implement this
	*/
	event* translateJoystickEvent(void* SDL_ev);

	event* translateTextEvent(void* SDL_ev);

	logMgr logger;
	uint8* sdl_kbstate;
	bool good;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
