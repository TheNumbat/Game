// Program Information ////////////////////////////////////////////////////////

/**
	@file cam.cpp

	@brief Declartion file for cam class

	Stores position of cam, TLC of screen.

	@author Max Slater

	@version 1.00 (28 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "cam.h"

#include <cmath>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

cam::cam()
	: pos(0,0,0,0,0,0)
{
	logger.StartLog("cam");
	zoom = 1;
}

cam::~cam()
{
	
}

bool cam::set(const map_position& newPos)
{
	pos = newPos;
	return true;
}

bool cam::set(std::weak_ptr<entity> e)
{
	if(e.expired())
	{
		logger.LogWarn("Trying to set cam position on nonexistant entity");
		return false;
	}

	if(!e.lock()->hasComponent(ctype_position))
	{
		logger.LogWarn("Trying to set cam position on entity without position");
		return false;
	}

	pos = std::static_pointer_cast<component_position>(e.lock()->getComponent(ctype_position).lock())->position;
	return true;
}

bool cam::move(const map_position& offset)
{
	pos += offset;
	pos.realChunkOffset = chunk_position(0,0,0);
	return true;
}

map_position cam::getCenter()
{
	return pos;
}

bool cam::updateFollow()
{
	if(!following.expired())
	{
		return set(following);
	}
	return false;
}

bool cam::setFollowing(std::weak_ptr<entity> e)
{
	if(e.expired())
	{
		logger.LogInfo("Clearing cam follow entity");
		following = std::weak_ptr<entity>();
		return true;
	}

	if(!e.lock()->hasComponent(ctype_position))
	{
		logger.LogWarn("Trying to set cam following entity without position");
		return false;
	}

	following = e;
	return true;
}

// Terminating precompiler directives  ////////////////////////////////////////
