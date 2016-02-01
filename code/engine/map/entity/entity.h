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
#include <vector>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Holds components to represent an entity.

	Used by mapMgr and elsewhere.
*/
class entity
{
public:
	ENGINE_API entity(uint64 _UID, uint32 time);
	ENGINE_API ~entity();

	ENGINE_API uint64 getUID() const;
	ENGINE_API uint32 getLastUpdate() const;
	ENGINE_API bool setLastUpdate(uint32 time);

	ENGINE_API bool hasComponent(component_type c);
	ENGINE_API std::shared_ptr<component>& addComponent(component_type c);
	ENGINE_API std::shared_ptr<component>& getComponent(component_type c);

private:
	std::vector<std::shared_ptr<component>> components;
	uint64 UID;
	uint32 lastUpdate;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
