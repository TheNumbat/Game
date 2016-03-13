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

class game_state;
class engine_state;

typedef void (*startupFunc)(void*,void*);
typedef startupFunc updateFunc;
typedef startupFunc shutdownFunc;

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief manages mod loading, unloading, and updating
*/
class modMgr
{
public:
	/**
		@brief modMgr constructor

		@param[in] g pointer to game state
		@param[in] e pointer to engine state
	*/
	modMgr(game_state* g, engine_state* e);

	/**
		@brief modMgr destructor

		Frees libraries
	*/
	~modMgr();

	/**
		@brief loads and initializes mods 

		Loads libraries and calls startup functions
	*/
	void startup();

	/**
		@brief updates mods as part of the game loop

		Calls update functions
	*/
	void updateMods();

	/**
		@brief deinitializes mods 

		Calls shutdown function
	*/
	void shutdown();

private:
	logMgr logger;

	std::vector<std::string> modLibIDs;

	std::vector<startupFunc> startupFuncs;
	std::vector<updateFunc> updateFuncs;
	std::vector<shutdownFunc> shutdownFuncs;

	game_state* game;
	engine_state* engine;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
