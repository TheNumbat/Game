// Program Information ////////////////////////////////////////////////////////

/**
	@file cam.h

	@brief Declartion file for cam class

	Stores position of cam, TLC of screen.

	@author Max Slater

	@version 1.00 (28 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "game_common.h"

#include "map\position\position.h"
#include "map\entity\entity.h"

#include <log\logMgr.h>
#include <memory>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Describes where the cam is pointing -- a position

	Can also move to an entity and follow an entity.
*/
struct cam
{
	/**
		@brief Default cam constructor.

		Sets position to 0,0,0,0,0, zoom to 1, following to NULL
	*/
	GAME_API cam();

	/**
		@brief cam destructor.

		Does nothing
	*/
	GAME_API ~cam();

	/**
		@brief Sets position to a new place

		@param[in] newPos new position to set to
	*/
	GAME_API bool set(const map_position& newPos);

	/**
		@brief Sets cam position to an entity

		Gets the position from the entity and uses it.

		@param[in] e pointer to entity to use. Should have position component

		@exception e NULL, will not update
		@exception e doesn't have position component, will not update
	*/
	GAME_API bool set(std::weak_ptr<entity> e);

	/**
		@brief Moves cam position by an offset

		Adds offset to position.

		@param[in] offset the movement to do
	*/
	GAME_API bool move(const map_position& offset);

	/**
		@brief Gets the cam position

		@return where the cam is positioned
	*/
	GAME_API map_position getCenter();

	/**
		@brief Updates cam position based on followed entity

		Adds offset to position.

		@note calls set
	*/
	GAME_API bool updateFollow();

	/**
		@brief Sets the cam following an entity

		@param[in] e is pointer to new entity to follow, or nothing to clear it

		@exception e does not have a position component, does nothing
	*/
	GAME_API bool setFollowing(std::weak_ptr<entity> e = std::weak_ptr<entity>());

	/// zoom
	real32 zoom;
	/// camera position
	map_position pos;
	/// followed entity
	std::weak_ptr<entity> following;
	/// logger
	logMgr logger;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
