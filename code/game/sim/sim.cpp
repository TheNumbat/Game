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

int simulate(void* data)
{
	game_state* game = ((simData*)data)->game;
	engine_state* engine = ((simData*)data)->engine;
	std::string timerID = ((simData*)data)->timerID;

	while(!((simData*)data)->exit)
	{
		std::weak_ptr<chunk> simChunk = game->map.getNextChunkForSim();

		if(!simChunk.expired())
		{
			std::lock_guard<std::recursive_mutex> lock(simChunk.lock()->lock);

			for(std::shared_ptr<entity> e : simChunk.lock()->entities)
			{
				if(e)
				{
					std::lock_guard<std::recursive_mutex> lock(e->lock);

					if(e->hasComponent(ctype_position) && e->hasComponent(ctype_movement))
					{
						std::weak_ptr<component_position> ePos = std::static_pointer_cast<component_position>(e->getComponent(ctype_position).lock());
						std::weak_ptr<component_movement> eMov = std::static_pointer_cast<component_movement>(e->getComponent(ctype_movement).lock());

						uint64 current = engine->time.get(timerID);
						uint64 dT = current - e->getLastUpdate();

						if(dT)
						{
							eMov.lock()->velocity += eMov.lock()->acceleration * (dT / 1000.0f);
							v2<real32> offset = eMov.lock()->velocity * (dT / 1000.0f);
							ePos.lock()->position += map_position(0,0,0,offset.x,offset.y,0);

							game->map.updateEntityMapPos(e);

							e->setLastUpdate(current);
						}
					}
				}

				if(!simChunk.lock()->entities.size())
				{
					break;
				}
			}
		}
	}

	return 0;
}
