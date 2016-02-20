// Program Information ////////////////////////////////////////////////////////

/**
	@file entity.cpp

	@brief Implementation file for entity class

	Entities hold components, it's a componenet entity system

	@author Max Slater

	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "entity.h"

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

entity::entity(uint32 _UID, uint32 time)
{
	UID = _UID;
	lastUpdate = time;
}

entity::~entity()
{

}

uint64 entity::getUID() const
{
	return UID;
}


uint32 entity::getLastUpdate() const
{
	return lastUpdate;
}

bool entity::setLastUpdate(uint32 time)
{
	lastUpdate = time;
	return true;
}

bool entity::hasComponent(component_type c) const
{
	if(components.find(c) == components.end())
	{
		return false;
	}
	return true;
}

std::weak_ptr<component> entity::addComponent(component_type c)
{
	if(hasComponent(c))
	{
		return getComponent(c);
	}

	std::shared_ptr<component> newC = createComponent(c);

	components.insert({c,newC});

	return std::weak_ptr<component>(newC);
}

std::weak_ptr<component> entity::getComponent(component_type c) const
{
	auto compEntry = components.find(c);
	if(compEntry == components.end())
	{
		return std::weak_ptr<component>();
	}
	return std::weak_ptr<component>(compEntry->second);
}

// Terminating precompiler directives  ////////////////////////////////////////
