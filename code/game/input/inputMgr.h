// Program Information ////////////////////////////////////////////////////////

/**
	@file inputMgr.h

	@brief Declartion file for the inputMgr class.

	See inputMgr.cpp for member function documentation

	@author Max Slater

	@version 1.00 (6 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "game_common.h"

#include <engine_state.h>

// Global constant definitions  ///////////////////////////////////////////////

class game_state;

enum inputstates
{
	input_none,
	input_gameplay,
	input_debugger
};

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief manages input for the game: depending on the input state events will
		   do different things
*/
class inputMgr
{
public:
	/**
		@brief mapMgr constructor

		@param[in] g pointer to game state
		@param[in] e pointer to engine state
	*/
	inputMgr(game_state* g, engine_state* e);

	/**
		@brief mapMgr constructor

		Does nothing;
	*/
	~inputMgr();

	/**
		@brief Handles all game input
	*/
	void handleEvents();

private:
	void handleGameplayEvent(const event& e);
	void handleDebugEvent(const event& e);

	inputstates inputstate;

	game_state* game;
	engine_state* engine;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
