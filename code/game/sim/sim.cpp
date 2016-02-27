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

#include "sim.h"

#include "game_state.h"

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

void simulate(engine_state* engine, game_state* game, const std::string& timerID)
{
	std::weak_ptr<chunk> simChunk;

	do
	{
		simChunk = game->map.getNextChunkForSim();

		if(!simChunk.expired())
		{
			for(auto entry : simChunk.lock()->entities)
			{
				std::weak_ptr<entity> e = entry.second;

				if(!e.expired())
				{
					if(e.lock()->hasComponent(ctype_position) && e.lock()->hasComponent(ctype_movement))
					{
						std::weak_ptr<component_position> ePos = std::static_pointer_cast<component_position>(e.lock()->getComponent(ctype_position).lock());
						std::weak_ptr<component_movement> eMov = std::static_pointer_cast<component_movement>(e.lock()->getComponent(ctype_movement).lock());

						uint64 current = engine->time.get(timerID);
						uint64 dT = current - e.lock()->getLastUpdate();

						if(dT)
						{
							eMov.lock()->velocity += eMov.lock()->acceleration * (dT / 1000.0f);
							v2<real32> offset = eMov.lock()->velocity * (dT / 1000.0f);
							ePos.lock()->position += map_position(0,0,0,offset.x,offset.y,0);

							e.lock()->setLastUpdate(current);

							if(game->map.updateEntityMapPos(e))
							{
								break;
							}
						}
					}
				}
			}
		}
	} 
	while(!simChunk.expired());
}
