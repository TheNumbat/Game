// Program Information ////////////////////////////////////////////////////////

/**
	@file mapMgr.cpp

	@brief Implementation file for the mapMgr class.

	@author Max Slater

	@version 1.00 (6 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "mapMgr.h"

#include <engine_state.h>
#include "game_state.h"

// Global constant definitions  ///////////////////////////////////////////////

const real32 fudge = 0.001f; // :(

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

mapMgr::mapMgr(game_state* g, engine_state* e, const std::string& timer)
{
	engine = e;
	game = g;
	timerID = timer;
	nextUnusedID = 1;
	logger.StartLog("MAP");
	logger.LogInfo("Map initialized");
}

mapMgr::~mapMgr()
{
	logger.LogInfo("Map uninitialized");
}

collision_pair::collision_pair(collision_class one, collision_class two)
{
	c1 = one;
	c2 = two;
}

collision_pair::~collision_pair()
{

}

bool collision_pair::operator==(const collision_pair& comp) const
{
	return (c1 == comp.c1 && c2 == comp.c2) ||
		   (c1 == comp.c2 && c2 == comp.c1);
}

void mapMgr::setCollisionRule(collision_class c1, collision_class c2, bool collides)
{
	collision_pair cp(c1,c2);
	auto entry = collisionRules.find(cp);
	if(entry == collisionRules.end())
	{
		collisionRules.insert({cp,collides});
	}
	else
	{
		entry->second = collides;
	}
}

bool mapMgr::getCollisionRule(collision_class c1, collision_class c2)
{
	collision_pair cp(c1,c2);
	auto entry = collisionRules.find(cp);
	if(entry == collisionRules.end())
	{
		return false;
	}
	else
	{
		return entry->second;
	}
}

void mapMgr::update()
{
	game->debug.beginProfiledFunc();

	std::weak_ptr<chunk> simChunk;

	if(engine->time.get(timerID) == -1)
	{
		engine->time.addTimer(timerID);
	}

	/// @todo update this to use simulation region around the player, and to not neccesarily sim the entire map every frame
	do
	{
		simChunk = getNextChunkForSim();

		if(!simChunk.expired())
		{
			bool moved;
			for(auto entry = simChunk.lock()->entities.begin(); entry != simChunk.lock()->entities.end(); moved ? entry : ++entry)
			{
				moved = false;
				std::weak_ptr<entity> e = entry->second;

				if(!e.expired())
				{
					uint32 UID = e.lock()->UID;
					uint64 current = engine->time.get(timerID);
					uint64 dT = current - e.lock()->getLastUpdate();

					if(e.lock()->hasComponent(ctype_position) && e.lock()->hasComponent(ctype_movement))
					{
						std::weak_ptr<component_position> ePos = std::static_pointer_cast<component_position>(e.lock()->getComponent(ctype_position).lock());
						std::weak_ptr<component_movement> eMov = std::static_pointer_cast<component_movement>(e.lock()->getComponent(ctype_movement).lock());

						while(dT > 0)
						{
							eMov.lock()->velocity += eMov.lock()->acceleration * (dT / 1000.0f); // @todo this is a problem, uses full dT
							v2<real32> dP = eMov.lock()->velocity * (dT / 1000.0f);

							if(!dP) break;

							segment<real32> sP(0,0,dP.x,dP.y);

							// collision
							// This is really pretty bad - needs to take into account collision rules,
							// more than just rects, STOP TUNNELING INTO THINGS
							std::vector<rect2<real32>> nearbyRects = getPossibleRects(ePos.lock()->position,dP,e.lock()->UID);
							for(rect2<real32> rect : nearbyRects)
							{
								std::weak_ptr<component_collision> eCol = std::static_pointer_cast<component_collision>(e.lock()->getComponent(ctype_collision).lock());

								for(rect2<real32> thisRect : eCol.lock()->cRects)
								{
									rect2<real32> expand = rect;//.sweep(thisRect);
									std::vector<segment<real32>> segments = expand.getSegments();

									v2<real32> closest = segments[0].intersect(sP);
									segment<real32> closestSeg = segments[0];
									for(int32 index = 1; index < segments.size(); index++)
									{
										v2<real32> newP = segments[index].intersect(sP);
										if(newP.length() < closest.length())
										{
											closest = newP;
											closestSeg = segments[index];
										}
									}
									
									if(closest == dP) // no collision
									{
										ePos.lock()->position += map_position(0,0,0,dP.x,dP.y,0);
										dT = 0;
									}
									else // collision
									{	
										// game->logger.LogInfo("x: " + std::to_string(closest.x) + " y: " + std::to_string(closest.y));
										ePos.lock()->position += map_position(0,0,0,closest.x,closest.y,0);	
										eMov.lock()->velocity = eMov.lock()->velocity.parallel(closestSeg.vec());
										dT *= std::abs(closest.x / dP.x);
									}
								}
							}
							if(!nearbyRects.size())
							{
								ePos.lock()->position += map_position(0,0,0,dP.x,dP.y,0);
								dT = 0;
							}
						}
					}

					e.lock()->setLastUpdate(current);
							
					if(updateEntityMapPos(e,false))
					{
						simChunk.lock()->entities.erase(entry++);
						moved = true;
					}
				}
			}
		}
	} 
	while(!simChunk.expired());

	game->debug.endProfiledFunc();
}

std::vector<rect2<real32>> mapMgr::getPossibleRects(const map_position& pos, const v2<real32>& dP, uint32 exclude)
{
	game->debug.beginProfiledFunc();

	std::vector<rect2<real32>> rects;

	v2<int32> area;
	if(dP.x >= 0) area.x = std::floor(dP.x / CHUNK_SIZE_METERS);
	else area.x = std::ceil(dP.x / CHUNK_SIZE_METERS);
	if(dP.y >= 0) area.y = std::floor(dP.y / CHUNK_SIZE_METERS);
	else area.y = std::ceil(dP.y / CHUNK_SIZE_METERS);

	for(int32 x = pos.chunkPos.x - area.x; x <= pos.chunkPos.x + area.x; x++)
	{
		for(int32 y = pos.chunkPos.y - area.y; y <= pos.chunkPos.y + area.y; y++)	
		{
			std::weak_ptr<chunk> current = getChunk(chunk_position(x,y,pos.chunkPos.z));
			if(!current.expired())
			{
				for(auto& entry : current.lock()->entities)
				{
					if(entry.second->UID != exclude)
					{
						std::weak_ptr<component_collision> eCol = std::static_pointer_cast<component_collision>(entry.second->getComponent(ctype_collision).lock());
						if(!eCol.expired())
						{
							std::weak_ptr<component_position> ePos = std::static_pointer_cast<component_position>(entry.second->getComponent(ctype_position).lock());
							for(auto& rectEntry : eCol.lock()->cRects)
							{
								rect2<real32> rect = rectEntry + getDistance(pos,ePos.lock()->position);
								rects.push_back(rect);
							}
						}
					}
				}
			}
		}
	}

	game->debug.endProfiledFunc();
	return rects;
}

v2<real32> mapMgr::getDistance(const map_position& one, const map_position& two)
{
	game->debug.beginProfiledFunc();
	v2<real32> relDistance( one.realPos.x - two.realPos.x , one.realPos.y - two.realPos.y );
	v2<real32> chunkDistance( one.chunkPos.x - two.chunkPos.x , one.chunkPos.y - two.chunkPos.y );
	game->debug.endProfiledFunc();
	return ( chunkDistance * CHUNK_SIZE_METERS ) + relDistance;
}

std::weak_ptr<entity> mapMgr::addEntity(const map_position& pos, std::string timerID)
{
	uint32 time = engine->time.get(timerID);
	if(time)
	{
		return addEntity(pos,time);
	}
	return std::weak_ptr<entity>();
}

std::weak_ptr<entity> mapMgr::addEntity(const map_position& pos, uint32 currentTimeMS)
{
	assert(nextUnusedID <= MAX_ENTITIES);
	if(nextUnusedID <= MAX_ENTITIES)
	{
		// Allocate entity
		std::shared_ptr<entity> newEntity = std::make_shared<entity>(nextUnusedID,currentTimeMS);

		// Set up position component
		std::weak_ptr<component_position> newEntityPos = std::static_pointer_cast<component_position>(newEntity->addComponent(ctype_position).lock());
		newEntityPos.lock()->position = pos;

		// Add entity to chunk
		std::weak_ptr<chunk> chunkEntry = addChunk(pos.chunkPos);
		chunkEntry.lock()->entities.insert({nextUnusedID,newEntity});

		// Success
		#ifdef VERBOSE_MAP
			logger.LogInfo("Added entity UID: " + std::to_string(nextUnusedID) + " at chunk x: " + std::to_string(pos.chunkPos.x) + 
						   " y: " + std::to_string(pos.chunkPos.y) + " z: " + std::to_string(pos.chunkPos.z));
		#endif

		nextUnusedID++;
		return std::weak_ptr<entity>(newEntity);
	}

	// Failure
	logger.LogFatal("Max entities exceeded!!!!");
	return std::weak_ptr<entity>();
}

std::weak_ptr<entity> mapMgr::addPlayer(const std::string& ID, const map_position& pos, std::string timerID)
{
	// Add entity
	std::weak_ptr<entity> newPlayer = addEntity(pos,timerID);

	// Add to player database
	if(!newPlayer.expired())
	{
		players.insert({ID,newPlayer});

		#ifdef VERBOSE_MAP
			logger.LogInfo("Added player ID: " + ID);
		#endif
	}

	return newPlayer;
}

std::weak_ptr<entity> mapMgr::addPlayer(const std::string& ID, const map_position& pos, uint32 currentTimeMS)
{
	// Add entity
	std::weak_ptr<entity> newPlayer = addEntity(pos,currentTimeMS);

	// Add to player database
	if(!newPlayer.expired())
	{
		players.insert({ID,newPlayer});

		#ifdef VERBOSE_MAP
			logger.LogInfo("Added player ID: " + ID);
		#endif
	}

	return newPlayer;
}

std::weak_ptr<entity> mapMgr::getPlayerByID(const std::string& ID)
{
	// Find player
	auto playerItem = players.find(ID);
	if(playerItem == players.end())
	{
		// Failure
		logger.LogWarn("Could not get player ID: " + ID + ", does not exist");
		return std::weak_ptr<entity>();
	}

	#ifdef VERY_VERBOSE_MAP
		logger.LogInfo("Got player ID: " + ID);
	#endif

	// Success, return player
	return playerItem->second;
}

std::weak_ptr<entity> mapMgr::getEntityByUID(uint32 UID)
{
	// Loop through chunks
	for(auto chunkEntry : map)
	{
		auto entry = chunkEntry.second->entities.find(UID);
		if(entry != chunkEntry.second->entities.end())
		{

			#ifdef VERBOSE_MAP
				logger.LogInfo("Got entity UID: " + std::to_string(UID));
			#endif

			return std::weak_ptr<entity>(entry->second);
		}
	}

	// Failure
	logger.LogWarn("Could not find entity by UID: " + std::to_string(UID) + ", does not exist");
	return std::weak_ptr<entity>();
}

bool mapMgr::removePlayer(const std::string& ID)
{
	// Find player
	auto playerItem = players.find(ID);
	if(playerItem == players.end())
	{
		// Failure
		logger.LogWarn("Could not remove player ID: " + ID + ", does not exist");
		return false;
	}


	#ifdef VERBOSE_MAP
		logger.LogInfo("Removed player ID: " + ID);
	#endif

	// Remove player 
	players.erase(playerItem);
	return removeEntity(playerItem->second);
}

bool mapMgr::removeEntity(const std::weak_ptr<entity>& e)
{
	// Test for position
	if(!e.lock()->hasComponent(ctype_position))
	{
		// Failure
		logger.LogWarn("Cannot remove entity ID: " + std::to_string(e.lock()->getUID()) + ", does not have position component");
		return false;
	}

	// Get chunk where the enity SHOULD be
	std::weak_ptr<chunk> chunkEntry = getChunkFromEntity(e);

	// assert chunk exists
	assert(!chunkEntry.expired());
	if(chunkEntry.expired())
	{
		// Failure
		logger.LogWarn("Cannot remove entity ID: " + std::to_string(e.lock()->getUID()) + ", references nonexistant chunk");
		return false;
	}

	#ifdef VERBOSE_MAP
		logger.LogInfo("Removed entity UID: " + e.lock()->getUID());
	#endif

	// Success, remove entity
	chunkEntry.lock()->entities.erase(e.lock()->getUID());
	return true;
}

bool mapMgr::removeEntityByUID(uint32 UID)
{
	// Loop through chunks
	for(auto chunkEntry : map)
	{
		auto entry = chunkEntry.second->entities.find(UID);
		if(entry != chunkEntry.second->entities.end())
		{

			#ifdef VERBOSE_MAP
				logger.LogInfo("Got entity UID: " + std::to_string(UID));
			#endif

			return removeEntity(entry->second);
		}
	}

	// Failure
	logger.LogWarn("Could not remove entity by UID: " + std::to_string(UID) + ", does not exist");
	return false;
}

bool mapMgr::updateEntityMapPos(const std::weak_ptr<entity>& e, bool remove)
{

	// Check that entity has component
	if(!e.lock()->hasComponent(ctype_position))
	{
		// Failure
		logger.LogWarn("Cannot update chunk position of entity UID: " + std::to_string(e.lock()->getUID()) + " - entity does not have position component!");
		return false;
	}

	std::weak_ptr<component_position> entityPos = std::static_pointer_cast<component_position>(e.lock()->getComponent(ctype_position).lock());

	if(entityPos.lock()->position.realChunkOffset == chunk_position(0,0,0))
	{
		// logger.LogWarn("Cannot update chunk position of entity UID: " + std::to_string(e.lock()->getUID()) + " - entity has not moved chunks!");
		return false;
	}

	// Get old/new chunks
	std::weak_ptr<chunk> oldChunk = getChunk(entityPos.lock()->position.chunkPos - entityPos.lock()->position.realChunkOffset);
	std::weak_ptr<chunk> newChunk = addChunk(entityPos.lock()->position.chunkPos);

	// Move entity
	newChunk.lock()->entities.insert({e.lock()->getUID(),e.lock()});

	if(remove)
	{
		oldChunk.lock()->entities.erase(e.lock()->getUID());
	}

	#ifdef VERBOSE_MAP
		logger.LogInfo("Updated chunk position of entity UID: " + std::to_string(e.lock()->getUID()));
	#endif

	// Success
	entityPos.lock()->position.realChunkOffset = chunk_position(0,0,0);
	return true;
}

std::weak_ptr<chunk> mapMgr::addChunk(const chunk_position& pos)
{
	// If chunk already exists, return it
	std::weak_ptr<chunk> testChunk = getChunk(pos);
	if(!testChunk.expired())
	{
		return testChunk;
	}

	// Create new chunk
	std::shared_ptr<chunk> newChunk = std::make_shared<chunk>();

	map.insert({pos,newChunk});

	#ifdef VERBOSE_MAP
		logger.LogInfo("Added chunk at x: " + std::to_string(pos.x) + " y: " + std::to_string(pos.y) + " z: " + std::to_string(pos.z));
	#endif

	return std::weak_ptr<chunk>(newChunk);
}

bool mapMgr::removeChunk(const chunk_position& pos)
{
	std::weak_ptr<chunk> chunkEntry = getChunk(pos);
	if(chunkEntry.expired())
	{
		// Failure
		logger.LogWarn("Cannot remove chunk at pos: " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z) + ", does not exist");
		return false;
	}

	// Success
	map.erase(pos);

	#ifdef VERBOSE_MAP
		logger.LogInfo("Removed chunk at x: " + std::to_string(pos.x) + " y: " + std::to_string(pos.y) + " z: " + std::to_string(pos.z));
	#endif

	return true;
}

std::weak_ptr<chunk> mapMgr::getChunk(const chunk_position& pos)
{
	auto chunkEntry = map.find(pos);

	if(chunkEntry == map.end())
	{
		// Failure
		return std::weak_ptr<chunk>();
	}

	// Success
	return std::weak_ptr<chunk>(chunkEntry->second);
}

std::weak_ptr<chunk> mapMgr::getChunkFromEntity(const std::weak_ptr<entity>& e)
{
	// Check that entity has component
	if(!e.lock()->hasComponent(ctype_position))
	{
		// Failure
		logger.LogWarn("Cannot get chunk from entity UID: " + std::to_string(e.lock()->getUID()) + " - entity does not have position component!");
		return std::weak_ptr<chunk>();
	}

	// Get chunk
	std::weak_ptr<chunk> chunkEntry = getChunk(std::static_pointer_cast<component_position>(e.lock()->getComponent(ctype_position).lock())->position.chunkPos);

	// assures that the enity is in the correct chunk; that the chunk position corresponds to the actual chunk it's in
	if(chunkEntry.expired() || chunkEntry.lock()->entities.find(e.lock()->getUID()) == chunkEntry.lock()->entities.end())
	{
		// Failure
		logger.LogWarn("Updating chunk position of entity UID: " + e.lock()->getUID());
		if(!updateEntityMapPos(e))
		{
			return std::weak_ptr<chunk>();
		}
		return getChunkFromEntity(e);
	}

	// Success
	return chunkEntry;
}

std::weak_ptr<chunk> mapMgr::getNextChunkForSim()
{
	// Test map
	if(map.empty())
	{
		return std::weak_ptr<chunk>();
	}

	static auto nextEntry = map.begin();

	// Wrap 
	if(nextEntry == map.end())
	{
		nextEntry = map.begin();
		return std::weak_ptr<chunk>();
	}

	// Get chunk
	std::weak_ptr<chunk> result = nextEntry->second;
	nextEntry++;

	return result;
}

// Terminating precompiler directives  ////////////////////////////////////////
