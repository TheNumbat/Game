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

#include <log\logMgr.h>
#include <map\position\position.h>
#include <map\entity\entity.h>

#include <memory>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Describes where the camera is pointing -- a position

	Can also move to an entity and follow an entity.
*/
class camera
{
public:
	/**
		@brief Default camera constructor.

		Sets position to 0,0,0,0,0, zoom to 1, following to NULL
	*/
	camera();

	/**
		@brief Camera destructor.

		Does nothing
	*/
	~camera();

	/**
		@brief Sets position to a new place

		@param[in] newPos new position to set to
	*/
	bool set(const map_position& newPos);

	/**
		@brief Sets camera position to an entity

		Gets the position from the entity and uses it.

		@param[in] e pointer to entity to use. Should have position component

		@exception e NULL, will not update
		@exception e doesn't have position component, will not update
	*/
	bool set(const std::weak_ptr<entity>& e);

	/**
		@brief Moves camera position by an offset

		Adds offset to position.

		@param[in] offset the movement to do
	*/
	bool move(const map_position& offset);

	/**
		@brief Gets the camera position

		@return where the camera is positioned
	*/
	map_position getCenter();

	/**
		@brief Updates camera position based on followed entity

		Adds offset to position.

		@note calls set
	*/
	bool updateFollow();

	/**
		@brief Sets the camera following an entity

		@param[in] e is pointer to new entity to follow, or nothing to clear it

		@exception e does not have a position component, does nothing
	*/
	bool setFollowing(const std::weak_ptr<entity>& e = std::weak_ptr<entity>());

	real32 zoom;

private:
	map_position pos;
	std::weak_ptr<entity> following;
	logMgr logger;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
