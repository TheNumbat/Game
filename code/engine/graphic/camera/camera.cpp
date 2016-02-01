// Program Information ////////////////////////////////////////////////////////

/**
	@file camera.cpp

	@brief Declartion file for camera class

	Stores position of camera, TLC of screen.

	@author Max Slater

	@version 1.00 (28 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "camera.h"

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

/**
	@brief Default camera constructor.

	Sets position to 0,0,0,0,0, zoom to 1, following to NULL
*/
camera::camera()
	: pos(0,0,0,0,0,0), following(NULL)
{
	zoom = 1;
}

/**
	@brief Camera destructor.

	Sets up logging.
*/
camera::~camera()
{
	logger.StartLog("CAMERA");
}

/**
	@brief Sets position to a new place

	@param[in] newPos new position to set to
*/
bool camera::set(const map_position& newPos)
{
	pos = newPos;
	return true;
}

/**
	@brief Sets camera position to an entity

	Gets the position from the entity and uses it.

	@param[in] e pointer to entity to use. Should have position component

	@exception e NULL, will not update
	@exception e doesn't have position component, will not update
*/
bool camera::set(std::shared_ptr<entity> e)
{
	if( !e )
	{
		logger.LogWarn("Trying to set camera position on NULL entity");
		return false;
	}

	if( !e->hasComponent(ctype_position) )
	{
		logger.LogWarn("Trying to set camera position on entity without position");
		return false;
	}

	pos = std::static_pointer_cast<component_position>(e->getComponent(ctype_position))->pos;
	return true;
}

/**
	@brief Moves camera position by an offset

	Adds offset to position.

	@param[in] offset the movement to do
*/
bool camera::move(const map_position& offset)
{
	pos += offset;
	return true;
}

/**
	@brief Updates camera position based on followed entity

	Adds offset to position.

	@note calls set
*/
bool camera::updateFollow()
{
	return set(following);
}

/**
	@brief Updates camera position based on followed entity

	Adds offset to position.

	@param[in] e is pointer to new entity to follow, or NULL to clear it

	@note calls set
*/
bool camera::setFollowing(std::shared_ptr<entity> e)
{
	if( !e->hasComponent(ctype_position) )
	{
		logger.LogWarn("Trying to set camera following entity without position");
		return false;
	}

	following = e;
	return true;
}

// Terminating precompiler directives  ////////////////////////////////////////
