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

#include "game_common.h"

#include "chunk/chunk.h"
#include "position/position.h"

#include <memory>
#include <unordered_map>
#include <vector>
#include <map>
#include <set>

// Global constant definitions  ///////////////////////////////////////////////

/// Chunk size in meters
const real32 CHUNK_SIZE_METERS = 25.0f;

struct game_state;
class engine_state;

// Class/Struct definitions  //////////////////////////////////////////////////

enum collision_class
{
	collision_player,
	collision_enviroment
};

struct collision_pair
{
	collision_pair(collision_class one, collision_class two);
	~collision_pair();
	bool operator==(const collision_pair& comp) const;
	collision_class c1;
	collision_class c2;
};

namespace std
{
	template<> 
	struct hash<collision_pair>
	{
		uint64 operator()(const collision_pair& value) const
		{
            uint64 h1 = std::hash<int32>()((int32)value.c1);
            uint64 h2 = std::hash<int32>()((int32)value.c2);

            return h2 ^ (h1 << 1);
		}
	};
}

/**
	@brief Describes an area of the map

	The mapMgr class manages all map and entity system related tasks, including
	storing the actual map. Each vector describes an area of the map, like a chunk.
	The "chunks" hold the "origional" shared_ptrs for the entities,
	and everything referencing entities should return a weak_ptr to these.

	Each "chunk" has a hashed chunk_position value, creating a sparsely stored map.

	@note More data will probably be added to the map in the future
*/
struct mapMgr
{
	/**
		@brief mapMgr constructor

		@param[in] e pointer to engine state
		@param[in] g pointer to game state
		@param[in] simTimer ID of timer to use for simulation

		Sets up log and starts entity IDs
	*/
	GAME_API mapMgr(game_state* g, engine_state* e, const std::string& simTimer);

	/**
		@brief mapMgr denstructor

		Does noting, entire map will delete itself
	*/
	GAME_API ~mapMgr();

	/**
		@brief Updates the map (runs physics)
	*/
	GAME_API void update();

	GAME_API std::vector<rect2<real32>> getPossibleRects(const map_position& pos, const v2<real32>& dP, uint32 exclude, collision_class currentClass);
	GAME_API v2<real32> getDistance(const map_position& one, const map_position& two);
	GAME_API void setCollisionRule(collision_class c1, collision_class c2, bool collides);
	GAME_API bool getCollisionRule(collision_class c1, collision_class c2);

	/**
		@brief creates a new entity

		Creates a new entity with only a position

		@param[in] pos position to add entity
		@param[in] currentTimeMS time to start the new entitie's lastUpdate at

		@return weak_ptr to new entity or nothing if failure

		@exception max entities exceeded, fatal error, returns weak_ptr to nothing
	*/
	GAME_API std::weak_ptr<entity> addEntity(const map_position& pos, uint32 currentTimeMS);

	/**
		@brief creates a new entity

		Creates a new entity with only a position

		@param[in] pos position to add entity
		@param[in] timeID id of timer to get current time from

		@return weak_ptr to new entity or nothing if failure

		@exception max entities exceeded, fatal error, returns weak_ptr to nothing
	*/
	GAME_API std::weak_ptr<entity> addEntity(const map_position& pos, std::string timerID);

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
	GAME_API std::weak_ptr<entity> addPlayer(const std::string& ID, const map_position& pos, uint32 currentTimeMS);

	/**
		@brief creates a new player entity

		Exact same as normally creating an entity, except it's added to the player
		storage for easy access.

		@param[in] ID of player to add
		@param[in] pos position to add player
		@param[in] timerID timer to get current time from

		@return weak_ptr to new player or nothing if failure

		@exception see addEntity()

		@note calls addEntity()
	*/
	GAME_API std::weak_ptr<entity> addPlayer(const std::string& ID, const map_position& pos, std::string timerID);

	/**
		@brief gets a player by it's ID (string name specificed on creation)

		@param[in] ID of player to return

		@return weak_ptr to player or nothing if not found

		@exception player does not exist, returns weak_ptr to nothing
	*/
	GAME_API std::weak_ptr<entity> getPlayerByID(const std::string& ID);

	/**
		@brief gets an entity by its UID -- searches through everything, very slow

		@param[in] UID of entity to find

		@return weak_ptr to entity or nothing if not found

		@exception entity does not exist, returns weak_ptr to nothing
	*/
	GAME_API std::weak_ptr<entity> getEntityByUID(uint32 UID);

	/**
		@brief removes a player and its entity

		@param[in] ID of player to removes

		@return success (bool)

		@exception player does not exist, returns false

		@note calls removeEntity()
	*/
	GAME_API bool removePlayer(const std::string& ID);

	/**
		@brief removes an entity

		@param[in] e pointer to entity to remove

		@return success (bool)

		@exception entity does not exist, returns false
	*/
	GAME_API bool removeEntity(const std::weak_ptr<entity>& e);

	/**
		@brief removes an entity by its UID -- searches through everything, very slow

		@param[in] UID of entity to remove

		@return success (bool)

		@exception entity does not exist, returns false
	*/
	GAME_API bool removeEntityByUID(uint32 UID);

	/**
		@brief updates an entity's position in the chunk map

		@note WILL NOT DELETE ENTITY IF REMOVE IS FALSE

		@param[in] e pointer to entity to update
		@param[in] remove whether or not to remove the enity from the chunk

		@return success

		@exception e does not have a position component, return false
	*/
	GAME_API bool updateEntityMapPos(const std::weak_ptr<entity>& e, bool remove = true);

	/**
		@brief creates a new chunk or returns one already in existance

		Call when you need to get a chunk no matter what

		@param[in] pos of chunk to add/get

		@return pointer to new/existing chunk

		@note if chunk already exists, it will be returned
	*/
	GAME_API std::weak_ptr<chunk> addChunk(const chunk_position& pos);

	/**
		@brief removes a chunk - USE WITH CARE!

		Will remove a chunk and all entities contained within

		@param[in] pos of chunk to remove

		@return success (bool)

		@exception if chunk does not exist, return false
	*/
	GAME_API std::weak_ptr<chunk> getChunk(const chunk_position& pos);

	/**
		@brief gets a chunk 

		Call when you need to get a chunk but don't want to add one

		@param[in] pos of chunk to get

		@return pointer to existing chunk or nothing

		@exception if chunk does not exist, return pointer to nothing
	*/
	GAME_API bool removeChunk(const chunk_position& pos);

	/**
		@brief returns a chunk that an entity (says its) in 

		@param[in] e pointer to entity to use

		@return pointer to chunk

		@exception entity does not have position component, return pointer to nothing
		@exception chunk does not exist, return pointer to nothing
		@exception found chunk does not actually include entity, return pointer to nothing
	*/
	GAME_API std::weak_ptr<chunk> getChunkFromEntity(const std::weak_ptr<entity>& e);

	/**
		@brief returns the next chunk for a simluation thread to run on.

		@return pointer to chunk

		@exception map is empty, return nullptr
	*/
	GAME_API std::weak_ptr<chunk> getNextChunkForSim();

	std::unordered_map<collision_pair,bool> collisionRules;

	/// Actual map of chunks
	std::unordered_map<chunk_position,std::shared_ptr<chunk>> map;
	/// Map of players for easy access
	std::map<std::string,std::weak_ptr<entity>> players;

	/// Next unused entity UID
	uint32 nextUnusedID;
	/// Logger
	logMgr logger;

	/// Pointer to engine
	engine_state* engine;
	/// Pointer to game
	game_state* game;
	/// ID of timer to use for simulation
	std::string timerID;

	/// Maximum number of entities that can exist
	static const uint32 MAX_ENTITIES = 1000000;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
