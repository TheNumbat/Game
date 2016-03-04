// Program Information ////////////////////////////////////////////////////////

/**
	@file sim.h
	@author Max Slater

	@brief does simulation for the game, simulate is run on n threads

	@version 1.00 (18 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "game_common.h"

#include <string>

// Global constant definitions  ///////////////////////////////////////////////

class engine_state;
class game_state;

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

/**
	@brief Simulates the game map

	@param[in] engine pointer to engine state
	@param[in] game pointer to game state
	@param[in] timerID ID of timer to use 
*/
void simulate(engine_state* engine, game_state* game, const std::string& timerID);

// Free function implementation  //////////////////////////////////////////////
