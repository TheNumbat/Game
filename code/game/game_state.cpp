// Program Information ////////////////////////////////////////////////////////

/**
	@file game_state.cpp
	@author Max Slater

	@brief Describes the game state

	@version 2.00 (3 Mar 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

#include "game_state.h"

// Typedefs ///////////////////////////////////////////////////////////////////

class engine_state;

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

game_state::game_state(engine_state* e)
{
	engine = e;
	startup();
}

game_state::~game_state()
{
	shutdown();
}

bool game_state::gameLoop()
{
	
}

void game_state::startup()
{

}

void game_state::shutdown()
{

}

// Free function prototypes  //////////////////////////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
