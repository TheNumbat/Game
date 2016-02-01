// Program Information ////////////////////////////////////////////////////////

/**
	@file camera.h

	@brief Declartion file for camera class

	Stores position of camera, TLC of screen.

	@author Max Slater

	@version 1.00 (28 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include "log\logMgr.h"
#include "map\position\position.h"
#include "map\entity\entity.h"

#include <memory>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Describes where the camera is pointing -- a position

	Can also move to an entity and follow an entity.

	Used by graphicMgr.
*/
class camera
{
	camera();
	~camera();

	bool set(const map_position& newPos);
	bool set(std::shared_ptr<entity> e);
	bool move(const map_position& offset);

	bool updateFollow();
	bool setFollowing(std::shared_ptr<entity> e = std::shared_ptr<entity>(NULL));

	map_position pos;
	real32 zoom;
	std::shared_ptr<entity> following;
	logMgr logger;

	friend class graphicMgr;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
