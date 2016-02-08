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

#include "engine_common.h"
#include "map\component\component.h"
#include <memory>
#include <map>
#include <mutex>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Holds components to represent an entity.

	Used by mapMgr and elsewhere.

	@note ALWAYS ALWAYS ALWAYS lock the mutex member when reading OR writing to
		  the entity. ALSO remember not to interfere with locking in functions
		  and stuch.
*/
class entity
{
public:
	ENGINE_API entity(uint32 _UID, uint32 time);
	ENGINE_API ~entity();

	ENGINE_API uint64 getUID() const;
	ENGINE_API uint32 getLastUpdate() const;
	ENGINE_API bool setLastUpdate(uint32 time);

	ENGINE_API bool hasComponent(component_type c) const;
	ENGINE_API const std::weak_ptr<component>& addComponent(component_type c);
	ENGINE_API const std::weak_ptr<component>& getComponent(component_type c) const;

	std::mutex lock;
	
private:
	std::map<component_type,std::shared_ptr<component>> components;

	uint32 UID;
	uint32 lastUpdate;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
