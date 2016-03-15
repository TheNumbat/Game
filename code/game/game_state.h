// Program Information ////////////////////////////////////////////////////////

/**
	@file game_state.h
	@author Max Slater

	@brief Describes the game state

	@version 2.00 (3 Mar 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

#include "cam/cam.h"
#include "map/mapMgr.h"
#include "debug/debugMgr.h"
#include "input/inputMgr.h"
#include "render/renderMgr.h"
#include "mod/modMgr.h"

#include <log/logMgr.h>

// Typedefs ///////////////////////////////////////////////////////////////////

class engine_state;

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Describes the overall game state -- used to store any information
	used within the game logic but not the engine.

	@note should only be created once, in the main function of the game.
*/
struct game_state
{
	GAME_API game_state(engine_state* e);
	GAME_API ~game_state();

	GAME_API bool gameLoop();
	GAME_API void startup();
	GAME_API void shutdown();
	
	cam camera;
	mapMgr map;
	debugMgr debug;
	renderMgr render;
	inputMgr input;
	logMgr logger;
	modMgr mods;

	bool running;
	engine_state* engine;
};

// Free function prototypes  //////////////////////////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
