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

#include <engine_state.h>

// Global constant definitions  ///////////////////////////////////////////////

class game_state;

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Describes data needed to run a simulation thread

	Used as an intermediate between the game loop and sim function
*/
struct simData
{
	/// Set to true to exit the thread
	bool exit;
	/// Pointer to game state
	game_state* game;
	/// Poniter to engine state
	engine_state* engine;
	/// ID of timer to use for simulation
	std::string timerID;
	// std::string mapID; Will be used for multiple maps
};

// Free function prototypes  //////////////////////////////////////////////////

/**
	@brief Simulates the game map

	@param[in] data pointer to simData structure
*/
int simulate(void* data);

// Free function implementation  //////////////////////////////////////////////
