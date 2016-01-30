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

// Global constant definitions  ///////////////////////////////////////////////

const uint8 EVT_BAD = 0;
const uint8 EVT_WINDOW = 1;
const uint8 EVT_KEY = 2;
const uint8 EVT_MOUSE = 3;
const uint8 EVT_QUIT = 4;
const uint8 EVT_JOYSTICK = 5;

const uint32 KEY_0 = '0';
const uint32 KEY_1 = '1';
const uint32 KEY_2 = '2';
const uint32 KEY_3 = '3';
const uint32 KEY_4 = '4';
const uint32 KEY_5 = '5';
const uint32 KEY_6 = '6';
const uint32 KEY_7 = '7';
const uint32 KEY_8 = '8';
const uint32 KEY_9 = '9';
const uint32 KEY_A = 'A';
const uint32 KEY_B = 'B';
const uint32 KEY_C = 'C';
const uint32 KEY_D = 'D';
const uint32 KEY_E = 'E';
const uint32 KEY_F = 'F';
const uint32 KEY_G = 'G';
const uint32 KEY_H = 'H';
const uint32 KEY_I = 'I';
const uint32 KEY_J = 'J';
const uint32 KEY_K = 'K';
const uint32 KEY_L = 'L';
const uint32 KEY_M = 'M';
const uint32 KEY_N = 'N';
const uint32 KEY_O = 'O';
const uint32 KEY_P = 'P';
const uint32 KEY_Q = 'Q';
const uint32 KEY_R = 'R';
const uint32 KEY_S = 'S';
const uint32 KEY_T = 'T';
const uint32 KEY_U = 'U';
const uint32 KEY_V = 'V';
const uint32 KEY_W = 'W';
const uint32 KEY_X = 'X';
const uint32 KEY_Y = 'Y';
const uint32 KEY_Z = 'Z';
const uint32 KEY_TAB = '\t';
const uint32 KEY_ENTER = '\n';
const uint32 KEY_COMMA = ',';
const uint32 KEY_SLASH = '/';
const uint32 KEY_SPACE = ' ';
const uint32 KEY_PERIOD = '.';
const uint32 KEY_RBRACKET = ']';
const uint32 KEY_LBRACKET = '[';
const uint32 KEY_SEMICOLON = ';';
const uint32 KEY_EQUALS = '=';
const uint32 KEY_GRAVE = '`'; // `/~
const uint32 KEY_MINUS = '-';
const uint32 KEY_BACKSLASH = '\\'; // \ or |

const uint32 KEY_NP_0 = '0' + 1000; 
const uint32 KEY_NP_1 = '1' + 1000;
const uint32 KEY_NP_2 = '2' + 1000;
const uint32 KEY_NP_3 = '3' + 1000;
const uint32 KEY_NP_4 = '4' + 1000;
const uint32 KEY_NP_5 = '5' + 1000;
const uint32 KEY_NP_6 = '6' + 1000;
const uint32 KEY_NP_7 = '7' + 1000;
const uint32 KEY_NP_8 = '8' + 1000;
const uint32 KEY_NP_9 = '9' + 1000;
const uint32 KEY_NP_MULTIPLY = '*' + 1000;
const uint32 KEY_NP_DIVIDE = '/' + 1000;
const uint32 KEY_NP_ADD = '+' + 1000;
const uint32 KEY_NP_SUBTRACT = '-' + 1000;
const uint32 KEY_NP_PERIOD = '.' + 1000;
const uint32 KEY_NP_ENTER = 1500;

const uint32 KEY_BACKSPACE = 9000;
const uint32 KEY_CAPSLOCK = 9001;
const uint32 KEY_DELETE = 9002;
const uint32 KEY_DOWN = 9003;
const uint32 KEY_UP = 9004;
const uint32 KEY_LEFT = 9005;
const uint32 KEY_RIGHT = 9006;
const uint32 KEY_END = 9007;
const uint32 KEY_ESCAPE = 9008;
const uint32 KEY_F1 = 9009;
const uint32 KEY_F2 = 9010;
const uint32 KEY_F3 = 9011;
const uint32 KEY_F4 = 9012;
const uint32 KEY_F5 = 9013;
const uint32 KEY_F6 = 9014;
const uint32 KEY_F7 = 9015;
const uint32 KEY_F8 = 9016;
const uint32 KEY_F9 = 9017;
const uint32 KEY_F10 = 9018;
const uint32 KEY_F11 = 9019;
const uint32 KEY_F12 = 9020;
const uint32 KEY_HOME = 9021;
const uint32 KEY_INSERT = 9022;
const uint32 KEY_LALT = 9023;
const uint32 KEY_RALT = 9024;
const uint32 KEY_LCTRL = 9025;
const uint32 KEY_RCTRL = 9026;
const uint32 KEY_LSHIFT = 9027;
const uint32 KEY_RSHIFT = 9028;
const uint32 KEY_NUMLOCK = 9029;
const uint32 KEY_PGUP = 9030;
const uint32 KEY_PGDOWN = 9031;
const uint32 KEY_PRINTSCREEN = 9032;
const uint32 KEY_SCROLLLOCK = 9033;
const uint32 KEY_BAD = 10000;

// Keyboard flags
const uint32 FLAG_KEY_PRESS = 1<<0;
const uint32 FLAG_KEY_RELEASE = 1<<1;
const uint32 FLAG_KEY_LSHIFT = 1<<2;
const uint32 FLAG_KEY_RSHIFT = 1<<3;
const uint32 FLAG_KEY_SHIFT = FLAG_KEY_LSHIFT | FLAG_KEY_RSHIFT;
const uint32 FLAG_KEY_LCTRL = 1<<4;
const uint32 FLAG_KEY_RCTRL = 1<<5;
const uint32 FLAG_KEY_CTRL = FLAG_KEY_LCTRL | FLAG_KEY_RCTRL;
const uint32 FLAG_KEY_LALT = 1<<6;
const uint32 FLAG_KEY_RALT = 1<<7;
const uint32 FLAG_KEY_ALT = FLAG_KEY_LALT | FLAG_KEY_RALT;
const uint32 FLAG_KEY_NUMLOCK = 1<<8;
const uint32 FLAG_KEY_CAPSLOCK = 1<<9;

// Mouse flags
const uint32 FLAG_MOUSE_PRESS = 1<<0;
const uint32 FLAG_MOUSE_RELEASE = 1<<1;
const uint32 FLAG_MOUSE_BUTTON = FLAG_MOUSE_PRESS | FLAG_MOUSE_RELEASE; // button event; not movement event
const uint32 FLAG_MOUSE_MOVEMENT = 1<<2;
const uint32 FLAG_MOUSE_LEFT = 1<<3; // only for button events
const uint32 FLAG_MOUSE_RIGHT = 1<<4; // only for button events
const uint32 FLAG_MOUSE_MIDDLE = 1<<5; // only for button events
const uint32 FLAG_MOUSE_DOUBLE = 1<<6; // bit is set if it was a double-click; else, not set. only for button events.

const uint32 VAL_MOUSE_X = 0x0000ffff; // << 0
const uint32 VAL_MOUSE_Y = 0xffff0000; // << 16

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Describes an input event.

	Used by eventMgr

	@note value and flags mean different things depending on the type of event,
		  see constants above.
*/
class event
{
	event(uint8 t = EVT_BAD, uint32 v = 0, uint32 f = 0);
	~event();

	uint8 type;
	uint32 value;
	uint32 flags;

	operator bool();

	friend class eventMgr;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
