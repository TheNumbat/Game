// Program Information ////////////////////////////////////////////////////////

/**
	@file chunk.h

	@brief Declartion file for the chunk class.

	@author Max Slater

	@version 1.00 (6 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"
#include "map/entity/entity.h"

#include <map>
#include <memory>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Describes an area of the map

	Each "chunk" has a hashed chunk_position value, creating a sparsely stored map.

	@note More data will probably be added to the chunk in the future
*/
struct chunk
{
	chunk();
	~chunk();

	std::map<uint64,std::shared_ptr<entity>> entities;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
