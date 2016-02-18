// Program Information ////////////////////////////////////////////////////////

/**
	@file sim.cpp
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

int simulate(void* data)
{
	game_state* game = ((simData*)data)->game;
	engine_state* engine = ((simData*)data)->engine;
	std::string timerID = ((simData*)data)->timerID;

	while(!((simData*)data)->exit)
	{
		std::weak_ptr<chunk> simChunk = engine->map.getNextChunkForSim();

		if(!simChunk.expired())
		{
			for(std::weak_ptr<entity> e : *simChunk.lock())
			{
				std::lock_guard<std::mutex> lock(e.lock()->lock);

				if(e.lock()->hasComponent(ctype_position) && e.lock()->hasComponent(ctype_movement))
				{
					std::weak_ptr<component_position> ePos = std::static_pointer_case<component_position>(e.lock()->getComponent(ctype_position).lock());
					std::weak_ptr<compoennt_movement> eMov = std::static_pointer_case<compoennt_movement>(e.lock()->getComponent(ctype_movement).lock());

					uint64 dT = engine->time.getTime(timerID) - e.lock()->getLastUpdate();


				}
			}
		}
	}

	return 0;
}
