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

#include <string>

// Global constant definitions  ///////////////////////////////////////////////

struct game_state;

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
struct inputMgr
{
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


	void handleGameplayEvent(event* e);
	void handleDebugEvent(event* e);
	void handleTextEvent(event* e, const std::string& exclude);

	inputstates inputstate;

	std::string inputStr;

	game_state* game;
	engine_state* engine;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
