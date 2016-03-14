// Program Information ////////////////////////////////////////////////////////

/**
	@file entity.h

	@brief Declartion file for entity class

	Entities hold components, it's a componenet entity system

	@author Max Slater

	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "game_common.h"

#include "map\component\component.h"
#include <memory>
#include <map>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Holds components to represent an entity.

	Used by mapMgr and elsewhere.

	@note ALWAYS ALWAYS ALWAYS lock the mutex member when reading OR writing to
		  the entity. ALSO remember not to interfere with locking in functions
		  and stuch.
*/
struct entity
{
	/**
		@brief entity constructor

		Assigns values, starts with no components
	*/
	entity(uint32 _UID, uint32 time);

	/**
		@brief entity destructor

		Does nothing

		@note the shared_ptr to componenets will delete themselves UNLESS
			  they are still accessable somewhere else, in which case they
			  will delete when all copies go out of scope
	*/
	~entity();

	/**
		@brief gets entity UID

		@return entity UID
	*/
	uint64 getUID();

	/**
		@brief gets entity lastUpdate

		@return entity lastUpdate in milliseconds
	*/
	uint32 getLastUpdate();

	/**
		@brief sets entity lastUpdate

		@param[in] time new update time

		@return sucess
	*/
	bool setLastUpdate(uint32 time);

	/**
		@brief tests if entity contains component type

		@param[in] c component type to test for

		@return bool contains component
	*/
	bool hasComponent(component_type c);

	/**
		@brief adds a component to an entity

		@param[in] c component type to add

		@return component found/added

		@exception if entity already has component return it
	*/
	std::weak_ptr<component> addComponent(component_type c);

	/**
		@brief gets a component from an entity

		@param[in] c component type to get

		@return component found, or weak_ptr with no managed obejct if not found

		@exception if entity does not have component return NULL
	*/
	std::weak_ptr<component> getComponent(component_type c);

	std::map<component_type,std::shared_ptr<component>> components;

	uint32 UID;
	uint32 lastUpdate;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
