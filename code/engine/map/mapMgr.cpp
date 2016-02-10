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

#include "engine_common.h"

#include "map/mapMgr.h"

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

/**
	@brief mapMgr constructor

	Sets up log and starts entity IDs
*/
mapMgr::mapMgr()
{
	nextUnusedID = 1;
	logger.StartLog("MAP");
}

/**
	@brief mapMgr denstructor

	Does noting, entire map will delete itself
*/
mapMgr::~mapMgr()
{
	logger.LogInfo("Map uninitialized");
}

/**
	@brief creates a new entity

	Creates a new entity with only a position

	@param[in] pos position to add entity
	@param[in] currentTimeMS time to start the new entitie's lastUpdate at

	@return weak_ptr to new entity or nothing if failure

	@exception max entities exceeded, fatal error, returns weak_ptr to nothing
*/
std::weak_ptr<entity> mapMgr::addEntity(const map_position& pos, uint32 currentTimeMS)
{
	assert(nextUnusedID <= MAX_ENTITIES);
	if(nextUnusedID <= MAX_ENTITIES)
	{
		// Allocate entity
		std::shared_ptr<entity> newEntity = std::make_shared<entity>(nextUnusedID,currentTimeMS);

		// Lock entity
		std::lock_guard<std::mutex> lock(newEntity->lock);

		// Set up position component
		std::weak_ptr<component_position> newEntityPos = std::static_pointer_cast<component_position>(newEntity->addComponent(ctype_position).lock());
		newEntityPos.lock()->pos = pos;

		// Add entity to chunk
		std::weak_ptr<chunk> chunkEntry = addChunk(pos.chunkPos);
		chunkEntry.lock()->insert(newEntity);

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

/**
	@brief creates a new player entity

	Exact same as normally creating an entity, except it's added to the player
	storage for easy access.

	@param[in] ID of player to add
	@param[in] pos position to add player
	@param[in] currentTimeMS time to start the new player's lastUpdate at

	@return weak_ptr to new player or nothing if failure

	@exception see addEntity()

	@note calls addEntity()
*/
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

/**
	@brief gets a player by it's ID (string name specificed on creation)

	@param[in] ID of player to return

	@return weak_ptr to player or nothing if not found

	@exception player does not exist, returns weak_ptr to nothing
*/
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

	#ifdef VERBOSE_MAP
		logger.LogInfo("Got player ID: " + ID);
	#endif

	// Success, return player
	return playerItem->second;
}

/**
	@brief gets an entity by its UID -- searches through everything, very slow

	Don't use this except for debugging

	@param[in] UID of entity to find

	@return weak_ptr to entity or nothing if not found

	@exception entity does not exist, returns weak_ptr to nothing
*/
std::weak_ptr<entity> mapMgr::getEntityByUID_SLOW(uint32 UID)
{
	// Loop through chunks
	for(auto chunkEntry : map)
	{
		// Loop through entities
		for(std::shared_ptr<entity> e : *chunkEntry.second)
		{
			// Lock entity
			std::lock_guard<std::mutex> lock(e->lock);

			// Test entity
			if(e->getUID() == UID)
			{
				// Success
				#ifdef VERBOSE_MAP
					logger.LogInfo("Got entity UID: " + std::to_string(UID));
				#endif

				return std::weak_ptr<entity>(e);
			}
		}
	}

	// Failure
	logger.LogWarn("Could not find entity by UID: " + std::to_string(UID) + ", does not exist");
	return std::weak_ptr<entity>();
}

/**
	@brief removes a player and its entity

	@param[in] ID of player to remove

	@return success (bool)

	@exception player does not exist, returns false

	@note calls removeEntity()
*/
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

/**
	@brief removes an entity

	@param[in] e pointer to entity to remove

	@return success (bool)

	@exception entity does not exist, returns false
*/
bool mapMgr::removeEntity(const std::weak_ptr<entity>& e)
{
	// Lock entity
	std::lock_guard<std::mutex> lock(e.lock()->lock);

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
	chunkEntry.lock()->erase(e.lock());
	return true;
}

/**
	@brief removes an entity by its UID -- searches through everything, very slow

	Don't use this except for debugging

	@param[in] UID of entity to remove

	@return success (bool)

	@exception entity does not exist, returns false
*/
bool mapMgr::removeEntityByUID_SLOW(uint32 UID)
{
	// Loop through chunks
	for(auto chunkEntry : map)
	{
		// Loop through entities
		for(std::shared_ptr<entity> e : *chunkEntry.second)
		{
			e->lock.lock();

			// Test entity
			if(e->getUID() == UID)
			{
				e->lock.unlock();

				// Success
				return removeEntity(std::weak_ptr<entity>(e));
			}
		}
	}

	// Failure
	logger.LogWarn("Could not remove entity by UID: " + std::to_string(UID) + ", does not exist");
	return false;
}

/**
	@brief updates an entity's position in the chunk map

	@param[in] e pointer to entity to update

	@return success

	@exception e does not have a position component, return false
*/
bool mapMgr::updateEntityMapPos(const std::weak_ptr<entity>& e)
{
	std::lock_guard<std::mutex> lock(e.lock()->lock);

	// Check that entity has component
	if(!e.lock()->hasComponent(ctype_position))
	{
		// Failure
		logger.LogWarn("Cannot update chunk position of entity UID: " + std::to_string(e.lock()->getUID()) + " - entity does not have position component!");
		return false;
	}

	std::weak_ptr<component_position> entityPos = std::static_pointer_cast<component_position>(e.lock()->getComponent(ctype_position).lock());

	if(entityPos.lock()->pos.realChunkOffset == chunk_position(0,0,0))
	{
		logger.LogWarn("Cannot update chunk position of entity UID: " + std::to_string(e.lock()->getUID()) + " - entity has not moved chunks!");
		return false;
	}

	// Get old/new chunks
	std::weak_ptr<chunk> oldChunk = getChunk(entityPos.lock()->pos.chunkPos - entityPos.lock()->pos.realChunkOffset);
	std::weak_ptr<chunk> newChunk = addChunk(entityPos.lock()->pos.chunkPos);

	// Move entity
	oldChunk.lock()->erase(e.lock());
	newChunk.lock()->insert(e.lock());

	#ifdef VERBOSE_MAP
		logger.LogInfo("Updated chunk position of entity UID: " + std::to_string(e.lock()->getUID()));
	#endif

	// Success
	entityPos.lock()->pos.realChunkOffset = chunk_position(0,0,0);
	return true;
}

/**
	@brief creates a new chunk or returns one already in existance

	Call when you need to get a chunk no matter what

	@param[in] pos of chunk to add/get

	@return pointer to new/existing chunk

	@note if chunk already exists, it will be returned
*/
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

/**
	@brief removes a chunk - USE WITH CARE!

	Will remove a chunk and all entities contained within

	@param[in] pos of chunk to remove

	@return success (bool)

	@exception if chunk does not exist, return false
*/
bool mapMgr::removeChunk(const chunk_position& pos)
{
	auto chunkEntry = map.find(pos);
	if(chunkEntry == map.end())
	{
		// Failure
		logger.LogWarn("Cannot remove chunk at pos: " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z) + ", does not exist");
		return false;
	}

	// Success
	map.erase(chunkEntry);

	#ifdef VERBOSE_MAP
		logger.LogInfo("Removed chunk at x: " + std::to_string(pos.x) + " y: " + std::to_string(pos.y) + " z: " + std::to_string(pos.z));
	#endif

	return true;
}

/**
	@brief gets a chunk 

	Call when you need to get a chunk but don't want to add one

	@param[in] pos of chunk to get

	@return pointer to existing chunk or nothing

	@exception if chunk does not exist, return pointer to nothing
*/
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

/**
	@brief returns a chunk that an entity (says its) in 


	@param[in] e pointer to entity to use

	@return pointer to chunk

	@exception entity does not have position component, return pointer to nothing
	@exception chunk does not exist, return pointer to nothing
	@exception found chunk does not actually include entity, return pointer to nothing
*/
std::weak_ptr<chunk> mapMgr::getChunkFromEntity(const std::weak_ptr<entity>& e)
{
	std::lock_guard<std::mutex> lock(e.lock()->lock);

	// Check that entity has component
	if(!e.lock()->hasComponent(ctype_position))
	{
		// Failure
		logger.LogWarn("Cannot get chunk from entity UID: " + std::to_string(e.lock()->getUID()) + " - entity does not have position component!");
		return std::weak_ptr<chunk>();
	}

	// Get chunk
	std::weak_ptr<chunk> chunkEntry = getChunk(std::static_pointer_cast<component_position>(e.lock()->getComponent(ctype_position).lock())->pos.chunkPos);

	// assures that the enity is in the correct chunk; that the chunk position corresponds to the actual chunk it's in
	if(chunkEntry.expired() || chunkEntry.lock()->find(e.lock()) == chunkEntry.lock()->end())
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

// Terminating precompiler directives  ////////////////////////////////////////
