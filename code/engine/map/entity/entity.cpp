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

/**
	@brief entity constructor

	Assigns values, starts with no components
*/
entity::entity(uint64 _UID, uint32 time)
{
	UID = _UID;
	lastUpdate = time;
}

/**
	@brief entity destructor

	Does nothing

	@note the shared_ptr to componenets will delete themselves UNLESS
		  they are still accessable somewhere else, in which case they
		  will delete when all copies go out of scope
*/
entity::~entity()
{

}

/**
	@brief gets entity UID

	@return entity UID
*/
uint64 entity::getUID() const
{
	return UID;
}

/**
	@brief gets entity lastUpdate

	@return entity lastUpdate in milliseconds
*/
uint32 entity::getLastUpdate() const
{
	return lastUpdate;
}

/**
	@brief sets entity lastUpdate

	@param[in] time new update time

	@return sucess
*/
bool entity::setLastUpdate(uint32 time)
{
	lastUpdate = time;
	return true;
}

/**
	@brief tests if entity contains component type

	@param[in] c component type to test for

	@return bool contains component
*/
bool entity::hasComponent(component_type c)
{
	for(std::shared_ptr<component> comp : components)
	{
		if( comp->type = c )
		{
			return true;
		}
	}
	return false;
}

/**
	@brief adds a component to an entity

	@param[in] c component type to add

	@return component found/added

	@exception if entity already has component return it
*/
std::shared_ptr<component>& entity::addComponent(component_type c)
{
	if( hasComponent(c) )
	{
		return getComponent(c);
	}

	std::shared_ptr<component> newC = createComponent(c);

	components.push_back(newC);

	return newC;
}

/**
	@brief gets a component from an entity

	@param[in] c component type to get

	@return component found

	@exception if entity does not have component return NULL
*/
std::shared_ptr<component>& entity::getComponent(component_type c)
{
	for(std::shared_ptr<component> comp : components)
	{
		if( comp->type = c )
		{
			return comp;
		}
	}
	return std::shared_ptr<component>(NULL);
}

// Terminating precompiler directives  ////////////////////////////////////////
