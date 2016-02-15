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
	/**
		@brief mapMgr constructor

		Sets up log and starts entity IDs
	*/
	ENGINE_API mapMgr();

	/**
		@brief mapMgr denstructor

		Does noting, entire map will delete itself
	*/
	ENGINE_API ~mapMgr();
	
	/**
		@brief creates a new entity

		Creates a new entity with only a position

		@param[in] pos position to add entity
		@param[in] currentTimeMS time to start the new entitie's lastUpdate at

		@return weak_ptr to new entity or nothing if failure

		@exception max entities exceeded, fatal error, returns weak_ptr to nothing
	*/
	ENGINE_API std::weak_ptr<entity> addEntity(const map_position& pos, uint32 currentTimeMS);

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
	ENGINE_API std::weak_ptr<entity> addPlayer(const std::string& ID, const map_position& pos, uint32 currentTimeMS);

	/**
		@brief gets a player by it's ID (string name specificed on creation)

		@param[in] ID of player to return

		@return weak_ptr to player or nothing if not found

		@exception player does not exist, returns weak_ptr to nothing
	*/
	ENGINE_API std::weak_ptr<entity> getPlayerByID(const std::string& ID);

	/**
		@brief gets an entity by its UID -- searches through everything, very slow

		Don't use this except for debugging

		@param[in] UID of entity to find

		@return weak_ptr to entity or nothing if not found

		@exception entity does not exist, returns weak_ptr to nothing
	*/
	ENGINE_API std::weak_ptr<entity> getEntityByUID_SLOW(uint32 UID);

	/**
		@brief removes a player and its entity

		@param[in] ID of player to removes

		@return success (bool)

		@exception player does not exist, returns false

		@note calls removeEntity()
	*/
	ENGINE_API bool removePlayer(const std::string& ID);

	/**
		@brief removes an entity

		@param[in] e pointer to entity to remove

		@return success (bool)

		@exception entity does not exist, returns false
	*/
	ENGINE_API bool removeEntity(const std::weak_ptr<entity>& e);

	/**
		@brief removes an entity by its UID -- searches through everything, very slow

		Don't use this except for debugging

		@param[in] UID of entity to remove

		@return success (bool)

		@exception entity does not exist, returns false
	*/
	ENGINE_API bool removeEntityByUID_SLOW(uint32 UID);

	/**
		@brief updates an entity's position in the chunk map

		@param[in] e pointer to entity to update

		@return success

		@exception e does not have a position component, return false
	*/
	ENGINE_API bool updateEntityMapPos(const std::weak_ptr<entity>& e);

	/**
		@brief creates a new chunk or returns one already in existance

		Call when you need to get a chunk no matter what

		@param[in] pos of chunk to add/get

		@return pointer to new/existing chunk

		@note if chunk already exists, it will be returned
	*/
	ENGINE_API std::weak_ptr<chunk> addChunk(const chunk_position& pos);

	/**
		@brief removes a chunk - USE WITH CARE!

		Will remove a chunk and all entities contained within

		@param[in] pos of chunk to remove

		@return success (bool)

		@exception if chunk does not exist, return false
	*/
	ENGINE_API std::weak_ptr<chunk> getChunk(const chunk_position& pos);

	/**
		@brief gets a chunk 

		Call when you need to get a chunk but don't want to add one

		@param[in] pos of chunk to get

		@return pointer to existing chunk or nothing

		@exception if chunk does not exist, return pointer to nothing
	*/
	ENGINE_API bool removeChunk(const chunk_position& pos);

	/**
		@brief returns a chunk that an entity (says its) in 


		@param[in] e pointer to entity to use

		@return pointer to chunk

		@exception entity does not have position component, return pointer to nothing
		@exception chunk does not exist, return pointer to nothing
		@exception found chunk does not actually include entity, return pointer to nothing
	*/
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
