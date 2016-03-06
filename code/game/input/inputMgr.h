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

// Global constant definitions  ///////////////////////////////////////////////

class game_state;
class engine_state;

const uint8 input_gameplay = 1<<0;
const uint8 input_profiling = 1<<1;

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
	uint8 inputstates;

	game_state* game;
	engine_state* engine;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
