// Program Information ////////////////////////////////////////////////////////

/**
	@file modMgr.h

	@brief Declartion file for the modMgr class.

	See modMgr.cpp for member function documentation

	@author Max Slater

	@version 1.00 (6 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "game_common.h"

#include <vector>
#include <log/logMgr.h>

// Global constant definitions  ///////////////////////////////////////////////

struct game_state;
class engine_state;

/// Prototype of mod startup function
typedef void (*startupFunc)(void*,void*);
/// Prototype of mod update function
typedef startupFunc updateFunc;
/// Prototype of mod shutdown function
typedef startupFunc shutdownFunc;

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief manages mod loading, unloading, and updating
*/
struct modMgr
{
	/**
		@brief modMgr constructor

		@param[in] g pointer to game state
		@param[in] e pointer to engine state
	*/
	GAME_API modMgr(game_state* g, engine_state* e);

	/**
		@brief modMgr destructor

		Frees libraries
	*/
	GAME_API ~modMgr();

	/**
		@brief loads and initializes mods 

		Loads libraries and calls startup functions
	*/
	GAME_API void startup();

	/**
		@brief updates mods as part of the game loop

		Calls update functions
	*/
	GAME_API void updateMods();

	/**
		@brief deinitializes mods 

		Calls shutdown function
	*/
	GAME_API void shutdown();

	/// Logger
	logMgr logger;

	/// IDs of mod loaded libraries
	std::vector<std::string> modLibIDs;

	/// Mod startup functions
	std::vector<startupFunc> startupFuncs;
	/// Mod update functions
	std::vector<updateFunc> updateFuncs;
	/// Mod shutdown functions
	std::vector<shutdownFunc> shutdownFuncs;

	/// Pointer to game
	game_state* game;
	/// Pointer to engine
	engine_state* engine;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
