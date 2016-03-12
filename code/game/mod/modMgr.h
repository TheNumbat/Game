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
	@brief manages input for the game: depending on the input state events will
		   do different things
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

	void startup();

	void updateMods();

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
