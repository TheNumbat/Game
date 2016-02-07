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
const std::weak_ptr<entity>& mapMgr::addEntity(const map_position& pos, uint32 currentTimeMS)
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
const std::weak_ptr<entity>& mapMgr::addPlayer(const std::string& ID, const map_position& pos, uint32 currentTimeMS)
{
	// Add entity
	std::weak_ptr<entity> newPlayer = addEntity(pos,currentTimeMS);

	// Add to player database
	if(!newPlayer.expired())
	{
		players.insert({ID,newPlayer});
	}

	return newPlayer;
}

/**
	@brief gets a player by it's ID (string name specificed on creation)

	@param[in] ID of player to return

	@return weak_ptr to player or nothing if not found

	@exception player does not exist, returns weak_ptr to nothing
*/
const std::weak_ptr<entity>& mapMgr::getPlayerByID(const std::string& ID)
{
	// Find player
	auto playerItem = players.find(ID);
	if(playerItem == players.end())
	{
		// Failure
		logger.LogWarn("Could not get player ID: " + ID + ", does not exist");
		return std::weak_ptr<entity>();
	}

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
const std::weak_ptr<entity>& mapMgr::getEntityByUID_SLOW(uint32 UID)
{
	// Loop through chunks
	for(auto chunkEntry : map)
	{
		// Loop through entities
		for(std::shared_ptr<entity> e : chunkEntry.second)
		{
			// Lock entity
			std::lock_guard<std::mutex> lock(e->lock);

			// Test entity
			if(e->getUID() == UID)
			{
				// Success
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
		for(std::shared_ptr<entity> e : chunkEntry.second)
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


bool mapMgr::updateEntityMapPos(const std::weak_ptr<entity>& e)
{

}

const std::weak_ptr<chunk>& mapMgr::addChunk(const chunk_position& pos)
{

}

const std::weak_ptr<chunk>& mapMgr::getChunk(const chunk_position& pos)
{

}

const std::weak_ptr<chunk>& mapMgr::getChunkFromEntity(const std::weak_ptr<entity>& e)
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

	assert(!chunkEntry.expired());
	if(chunkEntry.expired())
	{
		// Failure
		logger.LogWarn("Cannot get chunk from entity UID: " + std::to_string(e.lock()->getUID()) + " - chunk does not exist!");
		return std::weak_ptr<chunk>();
	}

	// asserts that the enity is in the correct chunk; that the chunk position corresponds to the actual chunk it's in
	assert(chunkEntry.lock()->find(e.lock()) != chunkEntry.lock()->end());
	if(chunkEntry.lock()->find(e.lock()) == chunkEntry.lock()->end())
	{
		// Failure
		logger.LogWarn("Cannot get chunk from entity UID: " + std::to_string(e.lock()->getUID()) + " - not in correct chunk!");
		return std::weak_ptr<chunk>();
	}

	// Success
	return chunkEntry;
}

// Terminating precompiler directives  ////////////////////////////////////////
