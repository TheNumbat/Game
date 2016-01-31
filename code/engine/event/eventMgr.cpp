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
		// Set up SDL events
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
		// Quit SDL events
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

		// Get next event from SDL
		if( SDL_PollEvent(&sdlE) == 0 )
		{
			return event(EVT_BAD);
		}
		else
		{
			// Translate event
			return translateEvent(&sdlE);
		}
	}
	else
	{
		logger.LogWarn( "Uniniitialized -- trying to get next event" );
	}
}

/**
	@brief translates SDL_Event to event using specific translate functions

	@param[in] SDL_ev SDL_Event to translate

	@return translated event from individualed translate functions
*/
event& eventMgr::translateEvent(void* SDL_ev)
{
	SDL_Event* e = (SDL_Event*)SDL_ev;

	// Translate based on type
	switch(e->type)
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
			return translateJoystickEvent(e);
	}
}

/**
	@brief translates window SDL_Event to event

	@param[in] SDL_ev SDL_Event to translate

	@return translated event
*/
event& eventMgr::translateWindowEvent(void* SDL_ev)
{
	SDL_Event* e = (SDL_Event*)SDL_ev;

	event ret(EVT_WINDOW);

	// Get event type
	switch( e->window.event )
	{
		case SDL_WINDOWEVENT_SHOWN: ret.flags |= FLAG_WINDOW_SHOWN; break;
		case SDL_WINDOWEVENT_HIDDEN: ret.flags |= FLAG_WINDOW_HIDDEN; break;
		case SDL_WINDOWEVENT_EXPOSED: ret.flags |= FLAG_WINDOW_EXPOSED; break;
		case SDL_WINDOWEVENT_MINIMIZED: ret.flags |= FLAG_WINDOW_MINIMIZED; break;
		case SDL_WINDOWEVENT_MAXIMIZED: ret.flags |= FLAG_WINDOW_MAXIMIZED; break;
		case SDL_WINDOWEVENT_RESTORED: ret.flags |= FLAG_WINDOW_RESTORED; break;
		case SDL_WINDOWEVENT_ENTER: ret.flags |= FLAG_WINDOW_ENTER; break;
		case SDL_WINDOWEVENT_LEAVE: ret.flags |= FLAG_WINDOW_LEAVE; break;
		case SDL_WINDOWEVENT_FOCUS_GAINED: ret.flags |= FLAG_WINDOW_FOCUSED; break;
		case SDL_WINDOWEVENT_FOCUS_LOST: ret.flags |= FLAG_WINDOW_UNFOCUSED; break;
		case SDL_WINDOWEVENT_CLOSE: ret.flags |= FLAG_WINDOW_CLOSE; break;

		// Get window movement
		case SDL_WINDOWEVENT_MOVED:
			ret.flags |= FLAG_WINDOW_MOVED;

			/// @todo Not sure if this is correct
			ret.value |= (uint64)e->window.data1;
			ret.value |= (uint64)e->window.data2 << 32;

			break;

		case SDL_WINDOWEVENT_RESIZED:
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			ret.flags |= FLAG_WINDOW_RESIZED;

			/// @todo Not sure if this is correct
			ret.value |= (uint64)e->window.data1;
			ret.value |= (uint64)e->window.data2 << 32;

			break;

		default:
			logger.LogWarn("Could not translate window event " + e->window.event);
			ret.value = KEY_BAD;
			break;
	}

	return ret;
}

/**
	@brief translates joystick SDL_Event to event

	@param[in] SDL_ev SDL_Event to translate

	@return translated event

	@todo implement this, it does nothing
*/
event& eventMgr::translateJoystickEvent(void* SDL_ev)
{
	event ret(EVT_JOYSTICK);

	return ret;
}

/**
	@brief translates mouse SDL_Event to event

	@param[in] SDL_ev SDL_Event to translate

	@return translated event

	@note for mouse motion, the new absolute x/y (relative to window) is returned
		  rather than the relative movement
*/
event& eventMgr::translateMouseEvent(void* SDL_ev)
{
	SDL_Event* e = (SDL_Event*)SDL_ev;

	event ret(EVT_MOUSE);

	// Get type of event
	if( e->type == SDL_MOUSEBUTTONDOWN )
	{
		ret.flags |= FLAG_MOUSE_PRESS;
	}
	else if( e->type == SDL_MOUSEBUTTONUP )
	{
		ret.flags |= FLAG_MOUSE_RELEASE;
	}
	else if( e->type == SDL_MOUSEWHEEL )
	{
		ret.flags |= FLAG_MOUSE_WHEEL;

		// Get wheel movement
		/// @todo Not sure if this is correct
		ret.value |= (uint64)e->wheel.x;
		ret.value |= (uint64)e->wheel.y << 32;

		return ret;
	}
	else
	{
		ret.flags |= FLAG_MOUSE_MOTION;

		// Get mouse movement
		/// @todo Not sure if this is correct, or if it should be relative
		ret.value |= (uint64)e->motion.x;
		ret.value |= (uint64)e->motion.y << 32;

		return ret;
	}

	// Set up mouse buttons for PRESS or RELEASE
	switch(e->button.button)
	{
		case SDL_BUTTON_LEFT: ret.value = VAL_MOUSE_LEFT; break;
		case SDL_BUTTON_RIGHT: ret.value = VAL_MOUSE_RIGHT; break;
		case SDL_BUTTON_X1: ret.value = VAL_MOUSE_X1; break;
		case SDL_BUTTON_X2: ret.value = VAL_MOUSE_X2; break;
		default:
			logger.LogWarn("Could not translate mouse button " + e->button.button);
			ret.value = KEY_BAD;
			break;
	}

	// Finally, check if double click
	if( e->button.clicks == 2 )
	{
		ret.flags |= FLAG_MOUSE_DOUBLE;
	}

	return ret;
}

/**
	@brief translates keyboard SDL_Event to event

	@param[in] SDL_ev SDL_Event to translate

	@return translated event
*/
event& eventMgr::translateKeyboardEvent(void* SDL_ev)
{
	SDL_Event* e = (SDL_Event*)SDL_ev;

	event ret(EVT_KEY);

	// Set up modifiers
	if( e->key.state == SDL_PRESSED )
	{
		ret.flags |= FLAG_KEY_PRESS;
	}
	else if( e->key.state == SDL_RELEASED )
	{
		ret.flags |= FLAG_KEY_RELEASE;
	}
	if( e->key.repeat )
	{
		ret.flags |= FLAG_KEY_REPEAT;
	}
	if( e->key.keysym.mod & KMOD_LSHIFT )
	{
		ret.flags |= FLAG_KEY_LSHIFT;
	}
	if( e->key.keysym.mod & KMOD_RSHIFT )
	{
		ret.flags |= FLAG_KEY_RSHIFT;
	}
	if( e->key.keysym.mod & KMOD_LCTRL )
	{
		ret.flags |= FLAG_KEY_LCTRL;
	}
	if( e->key.keysym.mod & KMOD_RCTRL )
	{
		ret.flags |= FLAG_KEY_RCTRL;
	}
	if( e->key.keysym.mod & KMOD_LALT )
	{
		ret.flags |= FLAG_KEY_LALT;
	}
	if( e->key.keysym.mod & KMOD_RALT )
	{
		ret.flags |= FLAG_KEY_RALT;
	}
	if( e->key.keysym.mod & KMOD_LGUI )
	{
		ret.flags |= FLAG_KEY_LGUI;
	}
	if( e->key.keysym.mod & KMOD_RGUI )
	{
		ret.flags |= FLAG_KEY_RGUI;
	}
	if( e->key.keysym.mod & KMOD_NUM )
	{
		ret.flags |= FLAG_KEY_NUMLOCK;
	}
	if( e->key.keysym.mod & KMOD_CAPS )
	{
		ret.flags |= FLAG_KEY_CAPSLOCK;
	}
	if( e->key.keysym.mod & KMOD_MODE )
	{
		ret.flags |= FLAG_KEY_ALTGR;
	}

	// Set up key value
	/// @todo make sure this includes everything
	switch(e->key.keysym.sym)
	{
		case SDLK_0: ret.value = KEY_0; break;
		case SDLK_1: ret.value = KEY_1; break;
		case SDLK_2: ret.value = KEY_2; break;
		case SDLK_3: ret.value = KEY_3; break;
		case SDLK_4: ret.value = KEY_4; break;
		case SDLK_5: ret.value = KEY_5; break;
		case SDLK_6: ret.value = KEY_6; break;
		case SDLK_7: ret.value = KEY_7; break;
		case SDLK_8: ret.value = KEY_8; break;
		case SDLK_9: ret.value = KEY_9; break;
		case SDLK_a: ret.value = KEY_A; break;
		case SDLK_b: ret.value = KEY_B; break;
		case SDLK_c: ret.value = KEY_C; break;
		case SDLK_d: ret.value = KEY_D; break;
		case SDLK_e: ret.value = KEY_E; break;
		case SDLK_f: ret.value = KEY_F; break;
		case SDLK_g: ret.value = KEY_G; break;
		case SDLK_h: ret.value = KEY_H; break;
		case SDLK_i: ret.value = KEY_I; break;
		case SDLK_j: ret.value = KEY_J; break;
		case SDLK_k: ret.value = KEY_K; break;
		case SDLK_l: ret.value = KEY_L; break;
		case SDLK_m: ret.value = KEY_M; break;
		case SDLK_n: ret.value = KEY_N; break;
		case SDLK_o: ret.value = KEY_O; break;
		case SDLK_p: ret.value = KEY_P; break;
		case SDLK_q: ret.value = KEY_Q; break;
		case SDLK_r: ret.value = KEY_R; break;
		case SDLK_s: ret.value = KEY_S; break;
		case SDLK_t: ret.value = KEY_T; break;
		case SDLK_u: ret.value = KEY_U; break;
		case SDLK_v: ret.value = KEY_V; break;
		case SDLK_w: ret.value = KEY_W; break;
		case SDLK_x: ret.value = KEY_X; break;
		case SDLK_y: ret.value = KEY_Y; break;
		case SDLK_z: ret.value = KEY_Z; break;
		case SDLK_BACKSPACE: ret.value = KEY_BACKSPACE; break;
		case SDLK_CAPSLOCK: ret.value = KEY_CAPSLOCK; break;
		case SDLK_COMMA: ret.value = KEY_COMMA; break;
		case SDLK_BACKSLASH: ret.value = KEY_BACKSLASH; break;
		case SDLK_DELETE: ret.value = KEY_DELETE; break;
		case SDLK_DOWN: ret.value = KEY_DOWN; break;
		case SDLK_UP: ret.value = KEY_UP; break;
		case SDLK_LEFT: ret.value = KEY_LEFT; break;
		case SDLK_RIGHT: ret.value = KEY_RIGHT; break;
		case SDLK_END: ret.value = KEY_END; break;
		case SDLK_EQUALS: ret.value = KEY_EQUALS; break;
		case SDLK_ESCAPE: ret.value = KEY_ESCAPE; break;
		case SDLK_F1: ret.value = KEY_F1; break;
		case SDLK_F2: ret.value = KEY_F2; break;
		case SDLK_F3: ret.value = KEY_F3; break;
		case SDLK_F4: ret.value = KEY_F4; break;
		case SDLK_F5: ret.value = KEY_F5; break;
		case SDLK_F6: ret.value = KEY_F6; break;
		case SDLK_F7: ret.value = KEY_F7; break;
		case SDLK_F8: ret.value = KEY_F8; break;
		case SDLK_F9: ret.value = KEY_F9; break;
		case SDLK_F10: ret.value = KEY_F10; break;
		case SDLK_F11: ret.value = KEY_F11; break;
		case SDLK_F12: ret.value = KEY_F12; break;
		case SDLK_BACKQUOTE: ret.value = KEY_GRAVE; break;
		case SDLK_HOME: ret.value = KEY_HOME; break;
		case SDLK_INSERT: ret.value = KEY_INSERT; break;
		case SDLK_KP_0: ret.value = KEY_NP_0; break;
		case SDLK_KP_1: ret.value = KEY_NP_1; break;
		case SDLK_KP_2: ret.value = KEY_NP_2; break;
		case SDLK_KP_3: ret.value = KEY_NP_3; break;
		case SDLK_KP_4: ret.value = KEY_NP_4; break;
		case SDLK_KP_5: ret.value = KEY_NP_5; break;
		case SDLK_KP_6: ret.value = KEY_NP_6; break;
		case SDLK_KP_7: ret.value = KEY_NP_7; break;
		case SDLK_KP_8: ret.value = KEY_NP_8; break;
		case SDLK_KP_9: ret.value = KEY_NP_9; break;
		case SDLK_KP_DIVIDE: ret.value = KEY_NP_DIVIDE; break;
		case SDLK_KP_MULTIPLY: ret.value = KEY_NP_MULTIPLY; break;
		case SDLK_KP_MINUS: ret.value = KEY_NP_SUBTRACT; break;
		case SDLK_KP_PLUS: ret.value = KEY_NP_ADD; break;
		case SDLK_KP_ENTER: ret.value = KEY_NP_ENTER; break;
		case SDLK_KP_PERIOD: ret.value = KEY_NP_PERIOD; break;
		case SDLK_LALT: ret.value = KEY_LALT; break;
		case SDLK_RALT: ret.value = KEY_RALT; break;
		case SDLK_LCTRL: ret.value = KEY_LCTRL; break;
		case SDLK_RCTRL: ret.value = KEY_RCTRL; break;
		case SDLK_LSHIFT: ret.value = KEY_LSHIFT; break;
		case SDLK_RSHIFT: ret.value = KEY_RSHIFT; break;
		case SDLK_NUMLOCKCLEAR: ret.value = KEY_NUMLOCK; break;
		case SDLK_PAGEUP: ret.value = KEY_PGUP; break;
		case SDLK_PAGEDOWN: ret.value = KEY_PGDOWN; break;
		case SDLK_PRINTSCREEN: ret.value = KEY_PRINTSCREEN; break;
		case SDLK_RETURN: ret.value = KEY_ENTER; break;
		case SDLK_RIGHTBRACKET: ret.value = KEY_RBRACKET; break;
		case SDLK_LEFTBRACKET: ret.value = KEY_LBRACKET; break;
		case SDLK_MINUS: ret.value = KEY_MINUS; break;
		case SDLK_PERIOD: ret.value = KEY_PERIOD; break;
		case SDLK_SCROLLLOCK: ret.value = KEY_SCROLLLOCK; break;
		case SDLK_SEMICOLON: ret.value = KEY_SEMICOLON; break;
		case SDLK_SLASH: ret.value = KEY_SLASH; break;
		case SDLK_SPACE: ret.value = KEY_SPACE; break;
		case SDLK_TAB: ret.value = KEY_TAB; break;
		default: 
			logger.LogWarn("Could not translate key value " + e->key.keysym.sym);
			ret.value = KEY_BAD; 
			break;
	}

	return ret;
}

// Terminating precompiler directives  ////////////////////////////////////////
