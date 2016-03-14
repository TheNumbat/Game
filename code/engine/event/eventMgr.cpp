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

eventMgr::eventMgr()
{
	good = false;
	sdl_kbstate = NULL;
	logger.StartLog("EVENTS");
}

eventMgr::~eventMgr()
{
	kill();
}

bool eventMgr::init()
{
	if(!good)
	{
		// Set up SDL events
		int result = SDL_InitSubSystem(SDL_INIT_EVENTS);
		assert(result == 0);
		if(result < 0)
		{
			logger.LogFatal("Failed to initialize SDL events");
			return false;
		}

		sdl_kbstate = (uint8*)SDL_GetKeyboardState(NULL);
		if(!sdl_kbstate)
		{	
			kill();
			logger.LogFatal("Failed to get keyboard state");
			return false;
		}

		logger.LogInfo("Events initialized");

		// Success
		good = true;
		return true;
	}

	// Failure
	logger.LogWarn("Trying to reinitialize events");
	return false;
}

bool eventMgr::kill()
{
	if(good)
	{
		// Quit SDL events
		SDL_QuitSubSystem(SDL_INIT_EVENTS);

		logger.LogInfo("Events uninitialized");

		good = false;
		sdl_kbstate = NULL;
		return true;
	}

	// Failure
	logger.LogWarn("Trying to kill uninitialized events");
	return false;
}

event* eventMgr::getNextEvent()
{
	if(good)
	{
		SDL_Event sdlE;

		// Get next event from SDL
		if(SDL_PollEvent(&sdlE) == 0)
		{
			event* ret = new event(EVT_BAD);
			#ifdef VERBOSE_EVENTS
				logger.LogInfo("Did not get new event!");
			#endif
			return ret;
		}
		else
		{
			// Translate event
			event* ret = translateEvent(&sdlE);
			#ifdef VERBOSE_EVENTS
				logger.LogInfo("Got new event type: " + std::to_string(e.type) + " value: " + 
							   std::to_string(e.value) + " flags: " + std::to_string(e.flags));
			#endif
			return ret;
		}
	}

	// Failure
	logger.LogWarn( "Uniniitialized -- trying to get next event");
	return NULL;
}

bool eventMgr::keyPressed(uint64 keycode)
{
	if(!sdl_kbstate)
	{
		logger.LogWarn("Keyboard state not initialized!");
		return 0;
	}
	
	switch(keycode)
	{
		case KEY_0: return sdl_kbstate[SDL_SCANCODE_0];
		case KEY_1: return sdl_kbstate[SDL_SCANCODE_1];
		case KEY_2: return sdl_kbstate[SDL_SCANCODE_2];
		case KEY_3: return sdl_kbstate[SDL_SCANCODE_3];
		case KEY_4: return sdl_kbstate[SDL_SCANCODE_4];
		case KEY_5: return sdl_kbstate[SDL_SCANCODE_5];
		case KEY_6: return sdl_kbstate[SDL_SCANCODE_6];
		case KEY_7: return sdl_kbstate[SDL_SCANCODE_7];
		case KEY_8: return sdl_kbstate[SDL_SCANCODE_8];
		case KEY_9: return sdl_kbstate[SDL_SCANCODE_9];
		case KEY_A: return sdl_kbstate[SDL_SCANCODE_A];
		case KEY_B: return sdl_kbstate[SDL_SCANCODE_B];
		case KEY_C: return sdl_kbstate[SDL_SCANCODE_C];
		case KEY_D: return sdl_kbstate[SDL_SCANCODE_D];
		case KEY_E: return sdl_kbstate[SDL_SCANCODE_E];
		case KEY_F: return sdl_kbstate[SDL_SCANCODE_F];
		case KEY_G: return sdl_kbstate[SDL_SCANCODE_G];
		case KEY_H: return sdl_kbstate[SDL_SCANCODE_H];
		case KEY_I: return sdl_kbstate[SDL_SCANCODE_I];
		case KEY_J: return sdl_kbstate[SDL_SCANCODE_J];
		case KEY_K: return sdl_kbstate[SDL_SCANCODE_K];
		case KEY_L: return sdl_kbstate[SDL_SCANCODE_L];
		case KEY_M: return sdl_kbstate[SDL_SCANCODE_M];
		case KEY_N: return sdl_kbstate[SDL_SCANCODE_N];
		case KEY_O: return sdl_kbstate[SDL_SCANCODE_O];
		case KEY_P: return sdl_kbstate[SDL_SCANCODE_P];
		case KEY_Q: return sdl_kbstate[SDL_SCANCODE_Q];
		case KEY_R: return sdl_kbstate[SDL_SCANCODE_R];
		case KEY_S: return sdl_kbstate[SDL_SCANCODE_S];
		case KEY_T: return sdl_kbstate[SDL_SCANCODE_T];
		case KEY_U: return sdl_kbstate[SDL_SCANCODE_U];
		case KEY_V: return sdl_kbstate[SDL_SCANCODE_V];
		case KEY_W: return sdl_kbstate[SDL_SCANCODE_W];
		case KEY_X: return sdl_kbstate[SDL_SCANCODE_X];
		case KEY_Y: return sdl_kbstate[SDL_SCANCODE_Y];
		case KEY_Z: return sdl_kbstate[SDL_SCANCODE_Z];
		case KEY_BACKSPACE: return sdl_kbstate[SDL_SCANCODE_BACKSPACE];
		case KEY_CAPSLOCK: return sdl_kbstate[SDL_SCANCODE_CAPSLOCK];
		case KEY_COMMA: return sdl_kbstate[SDL_SCANCODE_COMMA];
		case KEY_BACKSLASH: return sdl_kbstate[SDL_SCANCODE_BACKSLASH];
		case KEY_DELETE: return sdl_kbstate[SDL_SCANCODE_DELETE];
		case KEY_DOWN: return sdl_kbstate[SDL_SCANCODE_DOWN];
		case KEY_UP: return sdl_kbstate[SDL_SCANCODE_UP];
		case KEY_LEFT: return sdl_kbstate[SDL_SCANCODE_LEFT];
		case KEY_RIGHT: return sdl_kbstate[SDL_SCANCODE_RIGHT];
		case KEY_END: return sdl_kbstate[SDL_SCANCODE_END];
		case KEY_PLUS: return sdl_kbstate[SDL_SCANCODE_EQUALS];
		case KEY_ESCAPE: return sdl_kbstate[SDL_SCANCODE_ESCAPE];
		case KEY_F1: return sdl_kbstate[SDL_SCANCODE_F1];
		case KEY_F2: return sdl_kbstate[SDL_SCANCODE_F2];
		case KEY_F3: return sdl_kbstate[SDL_SCANCODE_F3];
		case KEY_F4: return sdl_kbstate[SDL_SCANCODE_F4];
		case KEY_F5: return sdl_kbstate[SDL_SCANCODE_F5];
		case KEY_F6: return sdl_kbstate[SDL_SCANCODE_F6];
		case KEY_F7: return sdl_kbstate[SDL_SCANCODE_F7];
		case KEY_F8: return sdl_kbstate[SDL_SCANCODE_F8];
		case KEY_F9: return sdl_kbstate[SDL_SCANCODE_F9];
		case KEY_F10: return sdl_kbstate[SDL_SCANCODE_F10];
		case KEY_F11: return sdl_kbstate[SDL_SCANCODE_F11];
		case KEY_F12: return sdl_kbstate[SDL_SCANCODE_F12];
		case KEY_GRAVE: return sdl_kbstate[SDL_SCANCODE_GRAVE];
		case KEY_HOME: return sdl_kbstate[SDL_SCANCODE_HOME];
		case KEY_INSERT: return sdl_kbstate[SDL_SCANCODE_INSERT];
		case KEY_NP_0: return sdl_kbstate[SDL_SCANCODE_KP_0];
		case KEY_NP_1: return sdl_kbstate[SDL_SCANCODE_KP_1];
		case KEY_NP_2: return sdl_kbstate[SDL_SCANCODE_KP_2];
		case KEY_NP_3: return sdl_kbstate[SDL_SCANCODE_KP_3];
		case KEY_NP_4: return sdl_kbstate[SDL_SCANCODE_KP_4];
		case KEY_NP_5: return sdl_kbstate[SDL_SCANCODE_KP_5];
		case KEY_NP_6: return sdl_kbstate[SDL_SCANCODE_KP_6];
		case KEY_NP_7: return sdl_kbstate[SDL_SCANCODE_KP_7];
		case KEY_NP_8: return sdl_kbstate[SDL_SCANCODE_KP_8];
		case KEY_NP_9: return sdl_kbstate[SDL_SCANCODE_KP_9];
		case KEY_NP_DIVIDE: return sdl_kbstate[SDL_SCANCODE_KP_DIVIDE];
		case KEY_NP_MULTIPLY: return sdl_kbstate[SDL_SCANCODE_KP_MULTIPLY];
		case KEY_NP_SUBTRACT: return sdl_kbstate[SDL_SCANCODE_KP_MINUS];
		case KEY_NP_ADD: return sdl_kbstate[SDL_SCANCODE_KP_PLUS];
		case KEY_NP_ENTER: return sdl_kbstate[SDL_SCANCODE_KP_ENTER];
		case KEY_NP_PERIOD: return sdl_kbstate[SDL_SCANCODE_KP_PERIOD];
		case KEY_LALT: return sdl_kbstate[SDL_SCANCODE_LALT];
		case KEY_RALT: return sdl_kbstate[SDL_SCANCODE_RALT];
		case KEY_LCTRL: return sdl_kbstate[SDL_SCANCODE_LCTRL];
		case KEY_RCTRL: return sdl_kbstate[SDL_SCANCODE_RCTRL];
		case KEY_LSHIFT: return sdl_kbstate[SDL_SCANCODE_LSHIFT];
		case KEY_RSHIFT: return sdl_kbstate[SDL_SCANCODE_RSHIFT];
		case KEY_NUMLOCK: return sdl_kbstate[SDL_SCANCODE_NUMLOCKCLEAR];
		case KEY_PGUP: return sdl_kbstate[SDL_SCANCODE_PAGEUP];
		case KEY_PGDOWN: return sdl_kbstate[SDL_SCANCODE_PAGEDOWN];
		case KEY_PRINTSCREEN: return sdl_kbstate[SDL_SCANCODE_PRINTSCREEN];
		case KEY_ENTER: return sdl_kbstate[SDL_SCANCODE_RETURN];
		case KEY_RBRACKET: return sdl_kbstate[SDL_SCANCODE_RIGHTBRACKET];
		case KEY_LBRACKET: return sdl_kbstate[SDL_SCANCODE_LEFTBRACKET];
		case KEY_MINUS: return sdl_kbstate[SDL_SCANCODE_MINUS];
		case KEY_PERIOD: return sdl_kbstate[SDL_SCANCODE_PERIOD];
		case KEY_SCROLLLOCK: return sdl_kbstate[SDL_SCANCODE_SCROLLLOCK];
		case KEY_SEMICOLON: return sdl_kbstate[SDL_SCANCODE_SEMICOLON];
		case KEY_SLASH: return sdl_kbstate[SDL_SCANCODE_SLASH];
		case KEY_SPACE: return sdl_kbstate[SDL_SCANCODE_SPACE];
		case KEY_TAB: return sdl_kbstate[SDL_SCANCODE_TAB];
		default: 
			logger.LogWarn("Invalid key code!");
			return 0;
	}
}

event* eventMgr::translateEvent(void* SDL_ev)
{
	SDL_Event* e = (SDL_Event*)SDL_ev;

	// Translate based on type
	switch(e->type)
	{
		// Quit
		case SDL_QUIT:
			return new event(EVT_QUIT);
		// Window
		case SDL_WINDOWEVENT:
			return translateWindowEvent(e);
		// Keyboard
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			return translateKeyboardEvent(e);
		// Mouse
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEWHEEL:
			return translateMouseEvent(e);
		// Joystick
		case SDL_JOYAXISMOTION:
		case SDL_JOYBALLMOTION:
		case SDL_JOYHATMOTION:
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
		case SDL_JOYDEVICEADDED:
		case SDL_JOYDEVICEREMOVED:
			return translateJoystickEvent(e);
		case SDL_TEXTINPUT:
			return translateTextEvent(e);
		// Other
		default:
			return new event(EVT_BAD);
	}
}

event* eventMgr::translateWindowEvent(void* SDL_ev)
{
	SDL_Event* e = (SDL_Event*)SDL_ev;

	event* ret = new event(EVT_WINDOW);

	// Get event type
	switch(e->window.event)
	{
		// Regular window events
		case SDL_WINDOWEVENT_SHOWN: ret->flags |= FLAG_WINDOW_SHOWN; break;
		case SDL_WINDOWEVENT_HIDDEN: ret->flags |= FLAG_WINDOW_HIDDEN; break;
		case SDL_WINDOWEVENT_EXPOSED: ret->flags |= FLAG_WINDOW_EXPOSED; break;
		case SDL_WINDOWEVENT_MINIMIZED: ret->flags |= FLAG_WINDOW_MINIMIZED; break;
		case SDL_WINDOWEVENT_MAXIMIZED: ret->flags |= FLAG_WINDOW_MAXIMIZED; break;
		case SDL_WINDOWEVENT_RESTORED: ret->flags |= FLAG_WINDOW_RESTORED; break;
		case SDL_WINDOWEVENT_ENTER: ret->flags |= FLAG_WINDOW_ENTER; break;
		case SDL_WINDOWEVENT_LEAVE: ret->flags |= FLAG_WINDOW_LEAVE; break;
		case SDL_WINDOWEVENT_FOCUS_GAINED: ret->flags |= FLAG_WINDOW_FOCUSED; break;
		case SDL_WINDOWEVENT_FOCUS_LOST: ret->flags |= FLAG_WINDOW_UNFOCUSED; break;
		case SDL_WINDOWEVENT_CLOSE: ret->flags |= FLAG_WINDOW_CLOSE; break;

		// Window movement
		case SDL_WINDOWEVENT_MOVED:
			ret->flags |= FLAG_WINDOW_MOVED;

			ret->value |= (uint64)e->window.data1;
			ret->value |= (uint64)e->window.data2 << 32;

			break;

		// Window size change
		case SDL_WINDOWEVENT_RESIZED:
			ret->flags |= FLAG_WINDOW_RESIZED;

			ret->value |= (uint64)e->window.data1;
			ret->value |= (uint64)e->window.data2 << 32;

			break;
		// Ignore this for now
		case SDL_WINDOWEVENT_SIZE_CHANGED: break;

		// Other
		default:
			logger.LogWarn("Could not translate window event");
			ret->value = KEY_BAD;
			break;
	}

	return ret;
}

event* eventMgr::translateJoystickEvent(void* SDL_ev)
{
	event* ret = new event(EVT_JOYSTICK);

	return ret;
}

event* eventMgr::translateMouseEvent(void* SDL_ev)
{
	SDL_Event* e = (SDL_Event*)SDL_ev;

	event* ret = new event(EVT_MOUSE);

	// Get type of event
	if(e->type == SDL_MOUSEBUTTONDOWN)
	{
		ret->flags |= FLAG_MOUSE_PRESS;
		ret->value |= (uint64)e->button.x;
		ret->value |= (uint64)e->button.y << 32;
	}
	else if(e->type == SDL_MOUSEBUTTONUP)
	{
		ret->flags |= FLAG_MOUSE_RELEASE;
		ret->value |= (uint64)e->button.x;
		ret->value |= (uint64)e->button.y << 32;
	}
	else if(e->type == SDL_MOUSEWHEEL)
	{
		ret->flags |= FLAG_MOUSE_WHEEL;

		// Get wheel movement
		ret->value |= (uint64)e->wheel.x;
		ret->value |= (uint64)e->wheel.y << 32;

		return ret;
	}
	else if(e->type == SDL_MOUSEMOTION)
	{
		ret->flags |= FLAG_MOUSE_MOTION;

		// Get mouse movement
		ret->value |= (uint64)e->motion.x;
		ret->value |= (uint64)e->motion.y << 32;

		return ret;
	}
	else
	{
		ret->value = KEY_BAD;
		return ret;
	}

	// Set up mouse buttons for PRESS or RELEASE
	switch(e->button.button)
	{
		case SDL_BUTTON_LEFT: ret->flags |= FLAG_MOUSE_LEFT; break;
		case SDL_BUTTON_RIGHT: ret->flags |= FLAG_MOUSE_RIGHT; break;
		case SDL_BUTTON_X1: ret->flags |= FLAG_MOUSE_X1; break;
		case SDL_BUTTON_X2: ret->flags |= FLAG_MOUSE_X2; break;
	}

	// Finally, check if double click
	if(e->button.clicks == 2)
	{
		ret->flags |= FLAG_MOUSE_DOUBLE;
	}

	return ret;
}

event* eventMgr::translateKeyboardEvent(void* SDL_ev)
{
	SDL_Event* e = (SDL_Event*)SDL_ev;

	event* ret = new event(EVT_KEY);

	// Set up modifiers
	if(e->key.state == SDL_PRESSED)
	{
		ret->flags |= FLAG_KEY_PRESS;
	}
	else if(e->key.state == SDL_RELEASED)
	{
		ret->flags |= FLAG_KEY_RELEASE;
	}
	if(e->key.repeat)
	{
		ret->flags = 0;
		ret->flags |= FLAG_KEY_REPEAT;
	}
	if(e->key.keysym.mod & KMOD_LSHIFT)
	{
		ret->flags |= FLAG_KEY_LSHIFT;
	}
	if(e->key.keysym.mod & KMOD_RSHIFT)
	{
		ret->flags |= FLAG_KEY_RSHIFT;
	}
	if(e->key.keysym.mod & KMOD_LCTRL)
	{
		ret->flags |= FLAG_KEY_LCTRL;
	}
	if(e->key.keysym.mod & KMOD_RCTRL)
	{
		ret->flags |= FLAG_KEY_RCTRL;
	}
	if(e->key.keysym.mod & KMOD_LALT)
	{
		ret->flags |= FLAG_KEY_LALT;
	}
	if(e->key.keysym.mod & KMOD_RALT)
	{
		ret->flags |= FLAG_KEY_RALT;
	}
	if(e->key.keysym.mod & KMOD_LGUI)
	{
		ret->flags |= FLAG_KEY_LGUI;
	}
	if(e->key.keysym.mod & KMOD_RGUI)
	{
		ret->flags |= FLAG_KEY_RGUI;
	}
	if(e->key.keysym.mod & KMOD_NUM)
	{
		ret->flags |= FLAG_KEY_NUMLOCK;
	}
	if(e->key.keysym.mod & KMOD_CAPS)
	{
		ret->flags |= FLAG_KEY_CAPSLOCK;
	}
	if(e->key.keysym.mod & KMOD_MODE)
	{
		ret->flags |= FLAG_KEY_ALTGR;
	}

	// Set up key value
	switch(e->key.keysym.sym)
	{
		case SDLK_0: ret->value = KEY_0; break;
		case SDLK_1: ret->value = KEY_1; break;
		case SDLK_2: ret->value = KEY_2; break;
		case SDLK_3: ret->value = KEY_3; break;
		case SDLK_4: ret->value = KEY_4; break;
		case SDLK_5: ret->value = KEY_5; break;
		case SDLK_6: ret->value = KEY_6; break;
		case SDLK_7: ret->value = KEY_7; break;
		case SDLK_8: ret->value = KEY_8; break;
		case SDLK_9: ret->value = KEY_9; break;
		case SDLK_a: ret->value = KEY_A; break;
		case SDLK_b: ret->value = KEY_B; break;
		case SDLK_c: ret->value = KEY_C; break;
		case SDLK_d: ret->value = KEY_D; break;
		case SDLK_e: ret->value = KEY_E; break;
		case SDLK_f: ret->value = KEY_F; break;
		case SDLK_g: ret->value = KEY_G; break;
		case SDLK_h: ret->value = KEY_H; break;
		case SDLK_i: ret->value = KEY_I; break;
		case SDLK_j: ret->value = KEY_J; break;
		case SDLK_k: ret->value = KEY_K; break;
		case SDLK_l: ret->value = KEY_L; break;
		case SDLK_m: ret->value = KEY_M; break;
		case SDLK_n: ret->value = KEY_N; break;
		case SDLK_o: ret->value = KEY_O; break;
		case SDLK_p: ret->value = KEY_P; break;
		case SDLK_q: ret->value = KEY_Q; break;
		case SDLK_r: ret->value = KEY_R; break;
		case SDLK_s: ret->value = KEY_S; break;
		case SDLK_t: ret->value = KEY_T; break;
		case SDLK_u: ret->value = KEY_U; break;
		case SDLK_v: ret->value = KEY_V; break;
		case SDLK_w: ret->value = KEY_W; break;
		case SDLK_x: ret->value = KEY_X; break;
		case SDLK_y: ret->value = KEY_Y; break;
		case SDLK_z: ret->value = KEY_Z; break;
		case SDLK_BACKSPACE: ret->value = KEY_BACKSPACE; break;
		case SDLK_CAPSLOCK: ret->value = KEY_CAPSLOCK; break;
		case SDLK_COMMA: ret->value = KEY_COMMA; break;
		case SDLK_BACKSLASH: ret->value = KEY_BACKSLASH; break;
		case SDLK_DELETE: ret->value = KEY_DELETE; break;
		case SDLK_DOWN: ret->value = KEY_DOWN; break;
		case SDLK_UP: ret->value = KEY_UP; break;
		case SDLK_LEFT: ret->value = KEY_LEFT; break;
		case SDLK_RIGHT: ret->value = KEY_RIGHT; break;
		case SDLK_END: ret->value = KEY_END; break;
		case SDLK_EQUALS: ret->value = KEY_PLUS; break;
		case SDLK_ESCAPE: ret->value = KEY_ESCAPE; break;
		case SDLK_F1: ret->value = KEY_F1; break;
		case SDLK_F2: ret->value = KEY_F2; break;
		case SDLK_F3: ret->value = KEY_F3; break;
		case SDLK_F4: ret->value = KEY_F4; break;
		case SDLK_F5: ret->value = KEY_F5; break;
		case SDLK_F6: ret->value = KEY_F6; break;
		case SDLK_F7: ret->value = KEY_F7; break;
		case SDLK_F8: ret->value = KEY_F8; break;
		case SDLK_F9: ret->value = KEY_F9; break;
		case SDLK_F10: ret->value = KEY_F10; break;
		case SDLK_F11: ret->value = KEY_F11; break;
		case SDLK_F12: ret->value = KEY_F12; break;
		case SDLK_BACKQUOTE: ret->value = KEY_GRAVE; break;
		case SDLK_HOME: ret->value = KEY_HOME; break;
		case SDLK_INSERT: ret->value = KEY_INSERT; break;
		case SDLK_KP_0: ret->value = KEY_NP_0; break;
		case SDLK_KP_1: ret->value = KEY_NP_1; break;
		case SDLK_KP_2: ret->value = KEY_NP_2; break;
		case SDLK_KP_3: ret->value = KEY_NP_3; break;
		case SDLK_KP_4: ret->value = KEY_NP_4; break;
		case SDLK_KP_5: ret->value = KEY_NP_5; break;
		case SDLK_KP_6: ret->value = KEY_NP_6; break;
		case SDLK_KP_7: ret->value = KEY_NP_7; break;
		case SDLK_KP_8: ret->value = KEY_NP_8; break;
		case SDLK_KP_9: ret->value = KEY_NP_9; break;
		case SDLK_KP_DIVIDE: ret->value = KEY_NP_DIVIDE; break;
		case SDLK_KP_MULTIPLY: ret->value = KEY_NP_MULTIPLY; break;
		case SDLK_KP_MINUS: ret->value = KEY_NP_SUBTRACT; break;
		case SDLK_KP_PLUS: ret->value = KEY_NP_ADD; break;
		case SDLK_KP_ENTER: ret->value = KEY_NP_ENTER; break;
		case SDLK_KP_PERIOD: ret->value = KEY_NP_PERIOD; break;
		case SDLK_LALT: ret->value = KEY_LALT; break;
		case SDLK_RALT: ret->value = KEY_RALT; break;
		case SDLK_LCTRL: ret->value = KEY_LCTRL; break;
		case SDLK_RCTRL: ret->value = KEY_RCTRL; break;
		case SDLK_LSHIFT: ret->value = KEY_LSHIFT; break;
		case SDLK_RSHIFT: ret->value = KEY_RSHIFT; break;
		case SDLK_NUMLOCKCLEAR: ret->value = KEY_NUMLOCK; break;
		case SDLK_PAGEUP: ret->value = KEY_PGUP; break;
		case SDLK_PAGEDOWN: ret->value = KEY_PGDOWN; break;
		case SDLK_PRINTSCREEN: ret->value = KEY_PRINTSCREEN; break;
		case SDLK_RETURN: ret->value = KEY_ENTER; break;
		case SDLK_RIGHTBRACKET: ret->value = KEY_RBRACKET; break;
		case SDLK_LEFTBRACKET: ret->value = KEY_LBRACKET; break;
		case SDLK_MINUS: ret->value = KEY_MINUS; break;
		case SDLK_PERIOD: ret->value = KEY_PERIOD; break;
		case SDLK_SCROLLLOCK: ret->value = KEY_SCROLLLOCK; break;
		case SDLK_SEMICOLON: ret->value = KEY_SEMICOLON; break;
		case SDLK_SLASH: ret->value = KEY_SLASH; break;
		case SDLK_SPACE: ret->value = KEY_SPACE; break;
		case SDLK_TAB: ret->value = KEY_TAB; break;
		default: 
			logger.LogWarn("Could not translate key value");
			ret->value = KEY_BAD; 
			break;
	}

	return ret;
}

event* eventMgr::translateTextEvent(void* SDL_ev)
{
	SDL_Event* e = (SDL_Event*)SDL_ev;
	return new text_event(e->text.text);
}

bool eventMgr::paste(std::string& text)
{
	char* str = SDL_GetClipboardText();
	if(!str)
	{
		logger.LogWarn((std::string)"Failed to get clipboar text, SDL_Error: " + SDL_GetError());
		return false;
	}
	text = str;
	return true;
}

bool eventMgr::copy(const std::string& text)
{
	int result = SDL_SetClipboardText(text.c_str());
	if(result != 0)
	{
		logger.LogWarn((std::string)"Failed to set clipboard text, SDL_Error: " + SDL_GetError());
		return false;
	}
	return true;
}

// Terminating precompiler directives  ////////////////////////////////////////
