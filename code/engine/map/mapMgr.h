// Program Information ////////////////////////////////////////////////////////

/**
	@file mapMgr.h

	@brief Declartion file for the mapMgr class.

	See mapMgr.cpp for member function documentation

	@author Max Slater

	@version 1.00 (6 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include "log/logMgr.h"
#include "entity/entity.h"
#include "map/position/position.h"

#include <vector>
#include <memory>
#include <set>
#include <unordered_map>

// Global constant definitions  ///////////////////////////////////////////////

/// Chunk concept
typedef std::set<std::shared_ptr<entity>> chunk;

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Describes an area of the map

	The mapMgr class manages all map and entity system related tasks, including
	storing the actual map. Each vector describes an area of the map, like a chunk.
	The "chunks" hold the "origional" shared_ptrs for the entities,
	and everything referencing entities should return a weak_ptr to these.

	Each "chunk" has a hashed chunk_position value, creating a sparsely stored map.

	@note More data will probably be added to the map in the future
*/
class mapMgr
{
public:
	ENGINE_API mapMgr();
	ENGINE_API ~mapMgr();

	// Entity operations
	ENGINE_API std::weak_ptr<entity> addEntity(const map_position& pos, uint32 currentTimeMS);
	ENGINE_API std::weak_ptr<entity> addPlayer(const std::string& ID, const map_position& pos, uint32 currentTimeMS);
	ENGINE_API std::weak_ptr<entity> getPlayerByID(const std::string& ID);
	ENGINE_API std::weak_ptr<entity> getEntityByUID_SLOW(uint32 UID);

	ENGINE_API bool removePlayer(const std::string& ID);
	ENGINE_API bool removeEntity(const std::weak_ptr<entity>& e);
	ENGINE_API bool removeEntityByUID_SLOW(uint32 UID);

	ENGINE_API bool updateEntityMapPos(const std::weak_ptr<entity>& e);

	// Chunk operations
	ENGINE_API std::weak_ptr<chunk> addChunk(const chunk_position& pos);
	ENGINE_API std::weak_ptr<chunk> getChunk(const chunk_position& pos);
	ENGINE_API bool removeChunk(const chunk_position& pos);
	ENGINE_API std::weak_ptr<chunk> getChunkFromEntity(const std::weak_ptr<entity>& e);

private:
	std::unordered_map<chunk_position,std::shared_ptr<chunk>> map;
	std::map<std::string,std::weak_ptr<entity>> players;

	uint32 nextUnusedID;
	logMgr logger;

	static const uint32 MAX_ENTITIES = 1000000;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
