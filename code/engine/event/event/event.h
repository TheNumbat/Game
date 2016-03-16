// Program Information ////////////////////////////////////////////////////////

/**
	@file event.h

	@brief Declartion file for event class.

	Event describes a single input event for the game, including keyboard,
	mouse, window, and windows events. Used by the event manager.

	See event.cpp for member function documentation

	@author Max Slater

	@version 1.00 (28 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include <string>

// Global constant definitions  ///////////////////////////////////////////////

// Event types
/// Could not translate event
const uint8 EVT_BAD = 0;
/// End of event stream
const uint8 EVT_END = 1;
/// Window event
const uint8 EVT_WINDOW = 2;
/// Keyboard event
const uint8 EVT_KEY = 3;
/// Mouse event
const uint8 EVT_MOUSE = 4;
/// Quit event
const uint8 EVT_QUIT = 5;
/// Gamepad event
const uint8 EVT_JOYSTICK = 6;
/// Text input event
const uint8 EVT_TEXT = 7;

// Key event values
/// 0 key
const uint64 KEY_0 = '0';
/// 1 key
const uint64 KEY_1 = '1';
/// 2 key
const uint64 KEY_2 = '2';
/// 3 key
const uint64 KEY_3 = '3';
/// 4 key
const uint64 KEY_4 = '4';
/// 5 key
const uint64 KEY_5 = '5';
/// 6 key
const uint64 KEY_6 = '6';
/// 7 key
const uint64 KEY_7 = '7';
/// 8 key
const uint64 KEY_8 = '8';
/// 9 key
const uint64 KEY_9 = '9';
/// A key
const uint64 KEY_A = 'A';
/// B key
const uint64 KEY_B = 'B';
/// C key
const uint64 KEY_C = 'C';
/// D key
const uint64 KEY_D = 'D';
/// E key
const uint64 KEY_E = 'E';
/// F key
const uint64 KEY_F = 'F';
/// G key
const uint64 KEY_G = 'G';
/// H key
const uint64 KEY_H = 'H';
/// I key
const uint64 KEY_I = 'I';
/// J key
const uint64 KEY_J = 'J';
/// K key
const uint64 KEY_K = 'K';
/// L key
const uint64 KEY_L = 'L';
/// M key
const uint64 KEY_M = 'M';
/// N key
const uint64 KEY_N = 'N';
/// O key
const uint64 KEY_O = 'O';
/// P key
const uint64 KEY_P = 'P';
/// Q key
const uint64 KEY_Q = 'Q';
/// R key
const uint64 KEY_R = 'R';
/// S key
const uint64 KEY_S = 'S';
/// T key
const uint64 KEY_T = 'T';
/// U key
const uint64 KEY_U = 'U';
/// V key
const uint64 KEY_V = 'V';
/// W key
const uint64 KEY_W = 'W';
/// X key
const uint64 KEY_X = 'X';
/// Y key
const uint64 KEY_Y = 'Y';
/// Z key
const uint64 KEY_Z = 'Z';
/// TAB key
const uint64 KEY_TAB = '\t';
/// GRAVE key
const uint64 KEY_GRAVE = '`'; // `/~
/// MINUS key
const uint64 KEY_MINUS = '-';
/// COMMA key
const uint64 KEY_COMMA = ',';
/// SLASH key
const uint64 KEY_SLASH = '/';
/// SPACE key
const uint64 KEY_SPACE = ' ';
/// PLUS key
const uint64 KEY_PLUS = '+';
/// ENTER key
const uint64 KEY_ENTER = '\n';
/// PERIOD key
const uint64 KEY_PERIOD = '.';
/// RBRACKET key
const uint64 KEY_RBRACKET = ']';
/// LBRACKET key
const uint64 KEY_LBRACKET = '[';
/// SEMICOLON key
const uint64 KEY_SEMICOLON = ';';
/// BACKSLASH key
const uint64 KEY_BACKSLASH = '\\'; // \ or |
//
/// NP_ENTER key
const uint64 KEY_NP_ENTER = 1500;
/// NP_0 key
const uint64 KEY_NP_0 = '0' + 1000; 
/// NP_1 key
const uint64 KEY_NP_1 = '1' + 1000;
/// NP_2 key
const uint64 KEY_NP_2 = '2' + 1000;
/// NP_3 key
const uint64 KEY_NP_3 = '3' + 1000;
/// NP_4 key
const uint64 KEY_NP_4 = '4' + 1000;
/// NP_5 key
const uint64 KEY_NP_5 = '5' + 1000;
/// NP_6 key
const uint64 KEY_NP_6 = '6' + 1000;
/// NP_7 key
const uint64 KEY_NP_7 = '7' + 1000;
/// NP_8 key
const uint64 KEY_NP_8 = '8' + 1000;
/// NP_9 key
const uint64 KEY_NP_9 = '9' + 1000;
/// NP_ADD key
const uint64 KEY_NP_ADD = '+' + 1000;
/// NP_PERIOD key
const uint64 KEY_NP_PERIOD = '.' + 1000;
/// NP_DIVIDE key
const uint64 KEY_NP_DIVIDE = '/' + 1000;
/// NP_MULTIPLY key
const uint64 KEY_NP_MULTIPLY = '*' + 1000;
/// NP_SUBTRACT key
const uint64 KEY_NP_SUBTRACT = '-' + 1000;

/// BACKSPACE key
const uint64 KEY_BACKSPACE = 9000;
/// CAPSLOCK key
const uint64 KEY_CAPSLOCK = 9001;
/// DELETE key
const uint64 KEY_DELETE = 9002;
/// DOWN key
const uint64 KEY_DOWN = 9003;
/// UP key
const uint64 KEY_UP = 9004;
/// LEFT key
const uint64 KEY_LEFT = 9005;
/// RIGHT key
const uint64 KEY_RIGHT = 9006;
/// END key
const uint64 KEY_END = 9007;
/// ESCAPE key
const uint64 KEY_ESCAPE = 9008;
/// F1 key
const uint64 KEY_F1 = 9009;
/// F2 key
const uint64 KEY_F2 = 9010;
/// F3 key
const uint64 KEY_F3 = 9011;
/// F4 key
const uint64 KEY_F4 = 9012;
/// F5 key
const uint64 KEY_F5 = 9013;
/// F6 key
const uint64 KEY_F6 = 9014;
/// F7 key
const uint64 KEY_F7 = 9015;
/// F8 key
const uint64 KEY_F8 = 9016;
/// F9 key
const uint64 KEY_F9 = 9017;
/// F10 key
const uint64 KEY_F10 = 9018;
/// F11 key
const uint64 KEY_F11 = 9019;
/// F12 key
const uint64 KEY_F12 = 9020;
/// HOME key
const uint64 KEY_HOME = 9021;
/// INSERT key
const uint64 KEY_INSERT = 9022;
/// LALT key
const uint64 KEY_LALT = 9023;
/// RALT key
const uint64 KEY_RALT = 9024;
/// LCTRL key
const uint64 KEY_LCTRL = 9025;
/// RCTRL key
const uint64 KEY_RCTRL = 9026;
/// LSHIFT key
const uint64 KEY_LSHIFT = 9027;
/// RSHIFT key
const uint64 KEY_RSHIFT = 9028;
/// NUMLOCK key
const uint64 KEY_NUMLOCK = 9029;
/// PGUP key
const uint64 KEY_PGUP = 9030;
/// PGDOWN key
const uint64 KEY_PGDOWN = 9031;
/// PRINTSCREEN key
const uint64 KEY_PRINTSCREEN = 9032;
/// SCROLLLOCK key
const uint64 KEY_SCROLLLOCK = 9033;
/// BAD key
const uint64 KEY_BAD = 10000;

// Mouse event value masks for wheel and motion
/// OUSE_XMASK flag
const uint64 VAL_MOUSE_XMASK = 0x00000000ffffffff; // << 0
/// OUSE_YMASK flag
const uint64 VAL_MOUSE_YMASK = 0xffffffff00000000; // << 32

// Window event value masks for wheel and motion
/// INDOW_XMASK flag
const uint64 VAL_WINDOW_XMASK = 0x00000000ffffffff; // << 0
/// INDOW_YMASK flag
const uint64 VAL_WINDOW_YMASK = 0xffffffff00000000; // << 32

// Keyboard event flags
/// KEY_PRESS flag
const uint32 FLAG_KEY_PRESS = 1<<0;
/// KEY_RELEASE flag
const uint32 FLAG_KEY_RELEASE = 1<<1;
/// KEY_REPEAT flag
const uint32 FLAG_KEY_REPEAT = 1<<2;
/// KEY_LSHIFT flag
const uint32 FLAG_KEY_LSHIFT = 1<<3;
/// KEY_RSHIFT flag
const uint32 FLAG_KEY_RSHIFT = 1<<4;
/// KEY_SHIFT flag
const uint32 FLAG_KEY_SHIFT = FLAG_KEY_LSHIFT | FLAG_KEY_RSHIFT;
/// KEY_LCTRL flag
const uint32 FLAG_KEY_LCTRL = 1<<5;
/// KEY_RCTRL flag
const uint32 FLAG_KEY_RCTRL = 1<<6;
/// KEY_CTRL flag
const uint32 FLAG_KEY_CTRL = FLAG_KEY_LCTRL | FLAG_KEY_RCTRL;
/// KEY_LALT flag
const uint32 FLAG_KEY_LALT = 1<<7;
/// KEY_RALT flag
const uint32 FLAG_KEY_RALT = 1<<8;
/// KEY_ALT flag
const uint32 FLAG_KEY_ALT = FLAG_KEY_LALT | FLAG_KEY_RALT;
/// KEY_LGUI flag
const uint32 FLAG_KEY_LGUI = 1<<9;
/// KEY_RGUI flag
const uint32 FLAG_KEY_RGUI = 1<<10;
/// KEY_GUI flag
const uint32 FLAG_KEY_GUI = FLAG_KEY_LGUI | FLAG_KEY_RGUI;
/// KEY_NUMLOCK flag
const uint32 FLAG_KEY_NUMLOCK = 1<<11;
/// KEY_CAPSLOCK flag
const uint32 FLAG_KEY_CAPSLOCK = 1<<12;
/// KEY_ALTGR flag
const uint32 FLAG_KEY_ALTGR = 1<<13;

// Mouse event flags
/// MOUSE_PRESS flag
const uint32 FLAG_MOUSE_PRESS = 1<<0;
/// MOUSE_RELEASE flag
const uint32 FLAG_MOUSE_RELEASE = 1<<1;
/// MOUSE_WHEEL flag
const uint32 FLAG_MOUSE_WHEEL = 1<<2;
/// MOUSE_BUTTON flag
const uint32 FLAG_MOUSE_BUTTON = FLAG_MOUSE_PRESS | FLAG_MOUSE_RELEASE; // button event; not movement event
/// MOUSE_MOTION flag
const uint32 FLAG_MOUSE_MOTION = 1<<3;
/// MOUSE_DOUBLE flag
const uint32 FLAG_MOUSE_DOUBLE = 1<<4; // bit is set if it was a double-click; else, not set. only for button events.

// Mouse event flags for buttons
/// MOUSE_LEFT flag
const uint64 FLAG_MOUSE_LEFT = 1<<5; 
/// MOUSE_RIGHT flag
const uint64 FLAG_MOUSE_RIGHT = 1<<6;
/// MOUSE_MIDDLE flag
const uint64 FLAG_MOUSE_MIDDLE = 1<<7;
/// MOUSE_X1 flag
const uint64 FLAG_MOUSE_X1 = 1<<8; 
/// MOUSE_X2 flag
const uint64 FLAG_MOUSE_X2 = 1<<9; 

// Window event flags
/// WINDOW_RESIZED flag
const uint32 FLAG_WINDOW_RESIZED = 1<<0;
/// WINDOW_MOVED flag
const uint32 FLAG_WINDOW_MOVED = 1<<1;
/// WINDOW_SHOWN flag
const uint32 FLAG_WINDOW_SHOWN = 1<<2;
/// WINDOW_HIDDEN flag
const uint32 FLAG_WINDOW_HIDDEN = 1<<3;
/// WINDOW_EXPOSED flag
const uint32 FLAG_WINDOW_EXPOSED = 1<<4;
/// WINDOW_MINIMIZED flag
const uint32 FLAG_WINDOW_MINIMIZED = 1<<5;
/// WINDOW_MAXIMIZED flag
const uint32 FLAG_WINDOW_MAXIMIZED = 1<<6;
/// WINDOW_RESTORED flag
const uint32 FLAG_WINDOW_RESTORED = 1<<7;
/// WINDOW_ENTER flag
const uint32 FLAG_WINDOW_ENTER = 1<<8;
/// WINDOW_LEAVE flag
const uint32 FLAG_WINDOW_LEAVE = 1<<9;
/// WINDOW_FOCUSED flag
const uint32 FLAG_WINDOW_FOCUSED = 1<<10;
/// WINDOW_UNFOCUSED flag
const uint32 FLAG_WINDOW_UNFOCUSED = 1<<11;
/// WINDOW_CLOSE flag
const uint32 FLAG_WINDOW_CLOSE = 1<<12;

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Describes an input event.

	Used by eventMgr

	@note value and flags mean different things depending on the type of event,
		  see constants above.
*/
struct event
{
	/**
		@brief event default/parameterized constructor.

		@param[in] t event type. See event type constants in event.h
		@param[in] v event value. See event type constants in event.h
		@param[in] f event flags. See event type constants in event.h
	*/
	ENGINE_API event(uint8 t = EVT_BAD, uint32 v = 0, uint32 f = 0);

	/**
		@brief event destructor (no detailed docs)

		Polymorphic
	*/
	ENGINE_API virtual ~event();

	/**
		@brief overloaded assignment operator

		assigns one event to another.
	*/
	ENGINE_API event& operator=(const event& src);

	/// event type, see EVT_type
	uint8 type;
	/// event value, see KEY_key or VAL_MOUSE_button or whatever
	uint64 value;
	/// event flag, see FLAG_KEY_modifier or whatever
	uint32 flags;
};

/**
	@brief text event

	Represents text input, inherits from event
*/
struct text_event : public event
{
	/**
		@brief text_event constructor

		@param[in] tIn text to use
	*/
	ENGINE_API text_event(const std::string& tIn);

	/**
		@brief rawTexture destructor
	*/
	ENGINE_API ~text_event();

	std::string text;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
