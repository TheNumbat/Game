#include "input.h"

#include <SDL.h>

Input::Input() {
	sdl_kbstate = NULL;
	good = false;
}

Input::~Input() {
	
}

bool Input::init() {
	logSetContext("INPUT");

	logInfo("Initializing input...");
	logEnterSec();

	if (good) {
		logWarn("Events already initialized");
		return false;
	}

	logInfo("Initializing event system");
	s32 result = SDL_InitSubSystem(SDL_INIT_EVENTS);
	assert(result == 0);
	if (result != 0) {
		logErr((std::string) "Failed to initialize SDL events,  Error: " + SDL_GetError());
		return false;
	}

	logInfo("Getting keyboard state");
	sdl_kbstate = (u8*)SDL_GetKeyboardState(NULL);
	assert(sdl_kbstate);
	if(!sdl_kbstate) {
		logErr((std::string) "Failed to get SDL keyboard state, Error: " + SDL_GetError());
		return false;
	}

	logInfo("Input initialized");
	logExitSec();
	good = true;
	return true;
}

bool Input::kill() {
	logSetContext("INPUT");

	if (!good) {
		logWarn("Input already destroyed");
		return false;
	}

	logInfo("Destroying input...");
	logEnterSec();

	logInfo("Quitting subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	logInfo("Done destroying input.");
	logExitSec();
	good = false;
	sdl_kbstate = NULL;
	return true;
}

event* Input::getNext() {
	logSetContext("INPUT");

	if (!good) {
		logWarn("Can't get event, input not initialized");
		return NULL;
	}

	SDL_Event e;
	if (SDL_PollEvent(&e) == 0) {
		return new event_endstream();
	} else {
		switch(e.type) {
			case SDL_QUIT:
				return new event_quit();
			case SDL_WINDOWEVENT:
				return translateWindow(&e);
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				return translateKey(&e);
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEWHEEL:
				return translateMouse(&e);
			case SDL_TEXTINPUT:
				return translateText(&e);
			default:
				return new event();	
		}
	}
}

bool Input::testKey(key k) {
	logSetContext("INPUT");

	if (!good) {
		logWarn("Can't test key, input not initialized");
		return false;
	}

#pragma warning ( push )
#pragma warning ( disable : 4800 )
	switch(k) {
		case key_0: return sdl_kbstate[SDL_SCANCODE_0];
		case key_1: return sdl_kbstate[SDL_SCANCODE_1];
		case key_2: return sdl_kbstate[SDL_SCANCODE_2];
		case key_3: return sdl_kbstate[SDL_SCANCODE_3];
		case key_4: return sdl_kbstate[SDL_SCANCODE_4];
		case key_5: return sdl_kbstate[SDL_SCANCODE_5];
		case key_6: return sdl_kbstate[SDL_SCANCODE_6];
		case key_7: return sdl_kbstate[SDL_SCANCODE_7];
		case key_8: return sdl_kbstate[SDL_SCANCODE_8];
		case key_9: return sdl_kbstate[SDL_SCANCODE_9];
		case key_a: return sdl_kbstate[SDL_SCANCODE_A];
		case key_b: return sdl_kbstate[SDL_SCANCODE_B];
		case key_c: return sdl_kbstate[SDL_SCANCODE_C];
		case key_d: return sdl_kbstate[SDL_SCANCODE_D];
		case key_e: return sdl_kbstate[SDL_SCANCODE_E];
		case key_f: return sdl_kbstate[SDL_SCANCODE_F];
		case key_g: return sdl_kbstate[SDL_SCANCODE_G];
		case key_h: return sdl_kbstate[SDL_SCANCODE_H];
		case key_i: return sdl_kbstate[SDL_SCANCODE_I];
		case key_j: return sdl_kbstate[SDL_SCANCODE_J];
		case key_k: return sdl_kbstate[SDL_SCANCODE_K];
		case key_l: return sdl_kbstate[SDL_SCANCODE_L];
		case key_m: return sdl_kbstate[SDL_SCANCODE_M];
		case key_n: return sdl_kbstate[SDL_SCANCODE_N];
		case key_o: return sdl_kbstate[SDL_SCANCODE_O];
		case key_p: return sdl_kbstate[SDL_SCANCODE_P];
		case key_q: return sdl_kbstate[SDL_SCANCODE_Q];
		case key_r: return sdl_kbstate[SDL_SCANCODE_R];
		case key_s: return sdl_kbstate[SDL_SCANCODE_S];
		case key_t: return sdl_kbstate[SDL_SCANCODE_T];
		case key_u: return sdl_kbstate[SDL_SCANCODE_U];
		case key_v: return sdl_kbstate[SDL_SCANCODE_V];
		case key_w: return sdl_kbstate[SDL_SCANCODE_W];
		case key_x: return sdl_kbstate[SDL_SCANCODE_X];
		case key_y: return sdl_kbstate[SDL_SCANCODE_Y];
		case key_z: return sdl_kbstate[SDL_SCANCODE_Z];
		case key_tab: return sdl_kbstate[SDL_SCANCODE_TAB];
		case key_grave: return sdl_kbstate[SDL_SCANCODE_GRAVE];
		case key_minus: return sdl_kbstate[SDL_SCANCODE_MINUS];
		case key_comma: return sdl_kbstate[SDL_SCANCODE_COMMA];
		case key_slash: return sdl_kbstate[SDL_SCANCODE_SLASH];
		case key_space: return sdl_kbstate[SDL_SCANCODE_SPACE];
		case key_plus: return sdl_kbstate[SDL_SCANCODE_EQUALS];
		case key_enter: return sdl_kbstate[SDL_SCANCODE_RETURN];
		case key_period: return sdl_kbstate[SDL_SCANCODE_PERIOD];
		case key_rbracket: return sdl_kbstate[SDL_SCANCODE_RIGHTBRACKET];
		case key_lbracket: return sdl_kbstate[SDL_SCANCODE_LEFTBRACKET];
		case key_semicolon: return sdl_kbstate[SDL_SCANCODE_SEMICOLON];
		case key_backslash: return sdl_kbstate[SDL_SCANCODE_BACKSLASH];
		case key_np_enter: return sdl_kbstate[SDL_SCANCODE_KP_ENTER];
		case key_np_0: return sdl_kbstate[SDL_SCANCODE_KP_0];
		case key_np_1: return sdl_kbstate[SDL_SCANCODE_KP_1];
		case key_np_2: return sdl_kbstate[SDL_SCANCODE_KP_2];
		case key_np_3: return sdl_kbstate[SDL_SCANCODE_KP_3];
		case key_np_4: return sdl_kbstate[SDL_SCANCODE_KP_4];
		case key_np_5: return sdl_kbstate[SDL_SCANCODE_KP_5];
		case key_np_6: return sdl_kbstate[SDL_SCANCODE_KP_6];
		case key_np_7: return sdl_kbstate[SDL_SCANCODE_KP_7];
		case key_np_8: return sdl_kbstate[SDL_SCANCODE_KP_8];
		case key_np_9: return sdl_kbstate[SDL_SCANCODE_KP_9];
		case key_np_add: return sdl_kbstate[SDL_SCANCODE_KP_PLUS];
		case key_np_period: return sdl_kbstate[SDL_SCANCODE_KP_PERIOD];
		case key_np_divide: return sdl_kbstate[SDL_SCANCODE_KP_DIVIDE];
		case key_np_multiply: return sdl_kbstate[SDL_SCANCODE_KP_MULTIPLY];
		case key_np_subtract: return sdl_kbstate[SDL_SCANCODE_KP_MINUS];
		case key_backspace: return sdl_kbstate[SDL_SCANCODE_BACKSPACE];
		case key_capslock: return sdl_kbstate[SDL_SCANCODE_CAPSLOCK];
		case key_delete: return sdl_kbstate[SDL_SCANCODE_DELETE];
		case key_down: return sdl_kbstate[SDL_SCANCODE_DOWN];
		case key_up: return sdl_kbstate[SDL_SCANCODE_UP];
		case key_left: return sdl_kbstate[SDL_SCANCODE_LEFT];
		case key_right: return sdl_kbstate[SDL_SCANCODE_RIGHT];
		case key_end: return sdl_kbstate[SDL_SCANCODE_END];
		case key_escape: return sdl_kbstate[SDL_SCANCODE_ESCAPE];
		case key_f1: return sdl_kbstate[SDL_SCANCODE_F1];
		case key_f2: return sdl_kbstate[SDL_SCANCODE_F2];
		case key_f3: return sdl_kbstate[SDL_SCANCODE_F3];
		case key_f4: return sdl_kbstate[SDL_SCANCODE_F4];
		case key_f5: return sdl_kbstate[SDL_SCANCODE_F5];
		case key_f6: return sdl_kbstate[SDL_SCANCODE_F6];
		case key_f7: return sdl_kbstate[SDL_SCANCODE_F7];
		case key_f8: return sdl_kbstate[SDL_SCANCODE_F8];
		case key_f9: return sdl_kbstate[SDL_SCANCODE_F9];
		case key_f10: return sdl_kbstate[SDL_SCANCODE_F10];
		case key_f11: return sdl_kbstate[SDL_SCANCODE_F11];
		case key_f12: return sdl_kbstate[SDL_SCANCODE_F12];
		case key_home: return sdl_kbstate[SDL_SCANCODE_HOME];
		case key_insert: return sdl_kbstate[SDL_SCANCODE_INSERT];
		case key_lalt: return sdl_kbstate[SDL_SCANCODE_LALT];
		case key_ralt: return sdl_kbstate[SDL_SCANCODE_RALT];
		case key_lctrl: return sdl_kbstate[SDL_SCANCODE_LCTRL];
		case key_rctrl: return sdl_kbstate[SDL_SCANCODE_RCTRL];
		case key_lshift: return sdl_kbstate[SDL_SCANCODE_LSHIFT];
		case key_rshift: return sdl_kbstate[SDL_SCANCODE_RSHIFT];
		case key_numlock: return sdl_kbstate[SDL_SCANCODE_NUMLOCKCLEAR];
		case key_pgup: return sdl_kbstate[SDL_SCANCODE_PAGEUP];
		case key_pgdown: return sdl_kbstate[SDL_SCANCODE_PAGEDOWN];
		case key_scrolllock: return sdl_kbstate[SDL_SCANCODE_SCROLLLOCK];
	}
#pragma warning ( pop )
	return false;
}

bool Input::copy(const std::string& text) {
	logSetContext("INPUT");

	s32 result = SDL_SetClipboardText(text.c_str());
	assert(result == 0);
	if (result != 0) {
		logWarn((std::string) "Failed to set clipboard text, Error: " + SDL_GetError());
		return false;
	}
	
	return true;
}

bool Input::paste(std::string& text) {
	logSetContext("INPUT");

	char* str = SDL_GetClipboardText();
	assert(str);
	if (!str) {
		logWarn((std::string) "Failed to get clipboard text, Error: " + SDL_GetError());
		return false;
	}

	text = str;
	return true;
}

void Input::startTextIn() {
	SDL_StartTextInput();
}

void Input::endTextIn() {
	SDL_StopTextInput();
}

event* Input::translateWindow(void* sdl_ev) {
	logSetContext("INPUT");

	SDL_Event* e = (SDL_Event*)sdl_ev;
	event_window* ret = new event_window();

	switch (e->window.event) {
		case SDL_WINDOWEVENT_SHOWN: ret->flags |= flag_window_shown; break;
		case SDL_WINDOWEVENT_HIDDEN: ret->flags |= flag_window_hidden; break;
		case SDL_WINDOWEVENT_EXPOSED: ret->flags |= flag_window_exposed; break;
		case SDL_WINDOWEVENT_MINIMIZED: ret->flags |= flag_window_minimized; break;
		case SDL_WINDOWEVENT_MAXIMIZED: ret->flags |= flag_window_maximized; break;
		case SDL_WINDOWEVENT_RESTORED: ret->flags |= flag_window_restored; break;
		case SDL_WINDOWEVENT_ENTER: ret->flags |= flag_window_enter; break;
		case SDL_WINDOWEVENT_LEAVE: ret->flags |= flag_window_leave; break;
		case SDL_WINDOWEVENT_FOCUS_GAINED: ret->flags |= flag_window_focused; break;
		case SDL_WINDOWEVENT_FOCUS_LOST: ret->flags |= flag_window_unfocused; break;
		case SDL_WINDOWEVENT_CLOSE: ret->flags |= flag_window_close; break;

		case SDL_WINDOWEVENT_MOVED:
			ret->flags |= flag_window_moved;

			ret->x = e->window.data1;
			ret->y = e->window.data2;

			break;

		case SDL_WINDOWEVENT_RESIZED:
			ret->flags |= flag_window_resized;

			ret->x = e->window.data1;
			ret->y = e->window.data2;

			break;

		case SDL_WINDOWEVENT_SIZE_CHANGED: 
			// TODO: what's the use of this?
			break;

		default:
			logWarn("Could not translate window event");
			delete ret;
			return new event();
			break;
	}

	return ret;
}

event* Input::translateKey(void* sdl_ev) {
	logSetContext("INPUT");

	SDL_Event* e = (SDL_Event*)sdl_ev;
	event_key* ret = new event_key();

	if (e->key.state == SDL_PRESSED) {
		ret->flags |= flag_key_press;
	} else if (e->key.state == SDL_RELEASED) {
		ret->flags |= flag_key_release;
	}
	if (e->key.repeat) {
		ret->flags = 0;
		ret->flags |= flag_key_repeat;
	}
	if (e->key.keysym.mod & KMOD_LSHIFT) {
		ret->flags |= flag_key_lshift;
	}
	if (e->key.keysym.mod & KMOD_RSHIFT) {
		ret->flags |= flag_key_rshift;
	}
	if (e->key.keysym.mod & KMOD_LCTRL) {
		ret->flags |= flag_key_lctrl;
	}
	if (e->key.keysym.mod & KMOD_RCTRL) {
		ret->flags |= flag_key_rctrl;
	}
	if (e->key.keysym.mod & KMOD_LALT) {
		ret->flags |= flag_key_lalt;
	}
	if (e->key.keysym.mod & KMOD_RALT) {
		ret->flags |= flag_key_ralt;
	}
	if (e->key.keysym.mod & KMOD_LGUI) {
		ret->flags |= flag_key_lgui;
	}
	if (e->key.keysym.mod & KMOD_RGUI) {
		ret->flags |= flag_key_rgui;
	}
	if (e->key.keysym.mod & KMOD_NUM) {
		ret->flags |= flag_key_numlock;
	}
	if (e->key.keysym.mod & KMOD_CAPS) {
		ret->flags |= flag_key_capslock;
	}
	if (e->key.keysym.mod & KMOD_MODE) {
		ret->flags |= flag_key_altgr;
	}

	switch(e->key.keysym.sym) {
		case SDLK_0: ret->k = key_0; break;
		case SDLK_1: ret->k = key_1; break;
		case SDLK_2: ret->k = key_2; break;
		case SDLK_3: ret->k = key_3; break;
		case SDLK_4: ret->k = key_4; break;
		case SDLK_5: ret->k = key_5; break;
		case SDLK_6: ret->k = key_6; break;
		case SDLK_7: ret->k = key_7; break;
		case SDLK_8: ret->k = key_8; break;
		case SDLK_9: ret->k = key_9; break;
		case SDLK_a: ret->k = key_a; break;
		case SDLK_b: ret->k = key_b; break;
		case SDLK_c: ret->k = key_c; break;
		case SDLK_d: ret->k = key_d; break;
		case SDLK_e: ret->k = key_e; break;
		case SDLK_f: ret->k = key_f; break;
		case SDLK_g: ret->k = key_g; break;
		case SDLK_h: ret->k = key_h; break;
		case SDLK_i: ret->k = key_i; break;
		case SDLK_j: ret->k = key_j; break;
		case SDLK_k: ret->k = key_k; break;
		case SDLK_l: ret->k = key_l; break;
		case SDLK_m: ret->k = key_m; break;
		case SDLK_n: ret->k = key_n; break;
		case SDLK_o: ret->k = key_o; break;
		case SDLK_p: ret->k = key_p; break;
		case SDLK_q: ret->k = key_q; break;
		case SDLK_r: ret->k = key_r; break;
		case SDLK_s: ret->k = key_s; break;
		case SDLK_t: ret->k = key_t; break;
		case SDLK_u: ret->k = key_u; break;
		case SDLK_v: ret->k = key_v; break;
		case SDLK_w: ret->k = key_w; break;
		case SDLK_x: ret->k = key_x; break;
		case SDLK_y: ret->k = key_y; break;
		case SDLK_z: ret->k = key_z; break;
		case SDLK_BACKSPACE: ret->k = key_backspace; break;
		case SDLK_CAPSLOCK: ret->k = key_capslock; break;
		case SDLK_COMMA: ret->k = key_comma; break;
		case SDLK_BACKSLASH: ret->k = key_backslash; break;
		case SDLK_DELETE: ret->k = key_delete; break;
		case SDLK_DOWN: ret->k = key_down; break;
		case SDLK_UP: ret->k = key_up; break;
		case SDLK_LEFT: ret->k = key_left; break;
		case SDLK_RIGHT: ret->k = key_right; break;
		case SDLK_END: ret->k = key_end; break;
		case SDLK_EQUALS: ret->k = key_plus; break;
		case SDLK_ESCAPE: ret->k = key_escape; break;
		case SDLK_F1: ret->k = key_f1; break;
		case SDLK_F2: ret->k = key_f2; break;
		case SDLK_F3: ret->k = key_f3; break;
		case SDLK_F4: ret->k = key_f4; break;
		case SDLK_F5: ret->k = key_f5; break;
		case SDLK_F6: ret->k = key_f6; break;
		case SDLK_F7: ret->k = key_f7; break;
		case SDLK_F8: ret->k = key_f8; break;
		case SDLK_F9: ret->k = key_f9; break;
		case SDLK_F10: ret->k = key_f10; break;
		case SDLK_F11: ret->k = key_f11; break;
		case SDLK_F12: ret->k = key_f12; break;
		case SDLK_BACKQUOTE: ret->k = key_grave; break;
		case SDLK_HOME: ret->k = key_home; break;
		case SDLK_INSERT: ret->k = key_insert; break;
		case SDLK_KP_0: ret->k = key_np_0; break;
		case SDLK_KP_1: ret->k = key_np_1; break;
		case SDLK_KP_2: ret->k = key_np_2; break;
		case SDLK_KP_3: ret->k = key_np_3; break;
		case SDLK_KP_4: ret->k = key_np_4; break;
		case SDLK_KP_5: ret->k = key_np_5; break;
		case SDLK_KP_6: ret->k = key_np_6; break;
		case SDLK_KP_7: ret->k = key_np_7; break;
		case SDLK_KP_8: ret->k = key_np_8; break;
		case SDLK_KP_9: ret->k = key_np_9; break;
		case SDLK_KP_DIVIDE: ret->k = key_np_divide; break;
		case SDLK_KP_MULTIPLY: ret->k = key_np_multiply; break;
		case SDLK_KP_MINUS: ret->k = key_np_subtract; break;
		case SDLK_KP_PLUS: ret->k = key_np_add; break;
		case SDLK_KP_ENTER: ret->k = key_np_enter; break;
		case SDLK_KP_PERIOD: ret->k = key_np_period; break;
		case SDLK_LALT: ret->k = key_lalt; break;
		case SDLK_RALT: ret->k = key_ralt; break;
		case SDLK_LCTRL: ret->k = key_lctrl; break;
		case SDLK_RCTRL: ret->k = key_rctrl; break;
		case SDLK_LSHIFT: ret->k = key_lshift; break;
		case SDLK_RSHIFT: ret->k = key_rshift; break;
		case SDLK_NUMLOCKCLEAR: ret->k = key_numlock; break;
		case SDLK_PAGEUP: ret->k = key_pgup; break;
		case SDLK_PAGEDOWN: ret->k = key_pgdown; break;
		case SDLK_RETURN: ret->k = key_enter; break;
		case SDLK_RIGHTBRACKET: ret->k = key_rbracket; break;
		case SDLK_LEFTBRACKET: ret->k = key_lbracket; break;
		case SDLK_MINUS: ret->k = key_minus; break;
		case SDLK_PERIOD: ret->k = key_period; break;
		case SDLK_SCROLLLOCK: ret->k = key_scrolllock; break;
		case SDLK_SEMICOLON: ret->k = key_semicolon; break;
		case SDLK_SLASH: ret->k = key_slash; break;
		case SDLK_SPACE: ret->k = key_space; break;
		case SDLK_TAB: ret->k = key_tab; break;
		default: 
			logWarn("Could not translate key value");
			delete ret;
			return new event();
	}

	return ret;
}

event* Input::translateMouse(void* sdl_ev) {
	logSetContext("INPUT");

	SDL_Event* e = (SDL_Event*)sdl_ev;
	event_mouse* ret = new event_mouse();

	switch(e->type) {
		case SDL_MOUSEBUTTONDOWN:
			ret->flags |= flag_mouse_press;
			ret->x = e->button.x;
			ret->y = e->button.y;
			break;
		case SDL_MOUSEBUTTONUP:
			ret->flags |= flag_mouse_release;
			ret->x = e->button.x;
			ret->y = e->button.y;
			break;
		case SDL_MOUSEWHEEL:
			ret->flags |= flag_mouse_wheel;
			ret->x = e->wheel.x;
			ret->y = e->wheel.y;
			return ret;
		case SDL_MOUSEMOTION:
			ret->flags |= flag_mouse_motion;
			ret->x = e->motion.x;
			ret->y = e->motion.y;
			return ret;
		default:
			logWarn("Failed to translate mouse event");
			delete ret;
			return new event();
	}

	switch (e->button.button) {
		case SDL_BUTTON_LEFT: ret->flags |= flag_mouse_lclick; break;
		case SDL_BUTTON_RIGHT: ret->flags |= flag_mouse_rclick; break;
		case SDL_BUTTON_X1: ret->flags |= flag_mouse_x1click; break;
		case SDL_BUTTON_X2: ret->flags |= flag_mouse_x2click; break;
	}

	if (e->button.clicks == 2) {
		ret->flags |= flag_mouse_double;
	}

	return ret;
}

event* Input::translateText(void* sdl_ev) {
	SDL_Event* e = (SDL_Event*)sdl_ev;
	event_text* ret = new event_text();
	ret->text = e->text.text;
	return ret;
}
