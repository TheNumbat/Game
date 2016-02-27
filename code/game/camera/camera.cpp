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

#include <cmath>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

camera::camera()
	: pos(0,0,0,0,0,0)
{
	logger.StartLog("CAMERA");
	zoom = 1;
}

camera::~camera()
{
	
}

bool camera::set(const map_position& newPos)
{
	pos = newPos;
	return true;
}

bool camera::set(std::weak_ptr<entity> e)
{
	if(e.expired())
	{
		logger.LogWarn("Trying to set camera position on nonexistant entity");
		return false;
	}

	if(!e.lock()->hasComponent(ctype_position))
	{
		logger.LogWarn("Trying to set camera position on entity without position");
		return false;
	}

	pos = std::static_pointer_cast<component_position>(e.lock()->getComponent(ctype_position).lock())->position;
	return true;
}

bool camera::move(const map_position& offset)
{
	pos += offset;
	pos.realChunkOffset = chunk_position(0,0,0);
	return true;
}

map_position camera::getCenter()
{
	return pos;
}

bool camera::updateFollow()
{
	if(!following.expired())
	{
		return set(following);
	}
	return false;
}

bool camera::setFollowing(std::weak_ptr<entity> e)
{
	if(e.expired())
	{
		logger.LogInfo("Clearing camera follow entity");
		following = std::weak_ptr<entity>();
		return true;
	}

	if(!e.lock()->hasComponent(ctype_position))
	{
		logger.LogWarn("Trying to set camera following entity without position");
		return false;
	}

	following = e;
	return true;
}

// Terminating precompiler directives  ////////////////////////////////////////
