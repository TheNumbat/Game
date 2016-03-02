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

#include "mapMgr.h"

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

mapMgr::mapMgr()
{
	nextUnusedID = 1;
	logger.StartLog("MAP");
	logger.LogInfo("Map initialized");
}

mapMgr::~mapMgr()
{
	logger.LogInfo("Map uninitialized");
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

bool mapMgr::updateEntityMapPos(const std::weak_ptr<entity>& e)
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