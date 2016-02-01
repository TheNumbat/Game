// Program Information ////////////////////////////////////////////////////////

/**
	@file component.h

	@brief Declartion file for coponent classes

	Components are held within entities

	@author Max Slater

	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include "map\position\position.h"
#include <memory>

// Global constant definitions  ///////////////////////////////////////////////

/**
	@brief Holds types of components

	Type value for each type of position
*/
enum component_type
{
	ctype_position = 0
};

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Base component struct for polymorphism

	Used by all components
*/
struct component
{
	component_type type;
};

/**
	@brief Position component

	Holds a position
*/
struct component_position : public component
{
	ENGINE_API component_position(map_position _pos = map_position(0,0,0,0,0,0));
	ENGINE_API ~component_position();

	map_position pos;
};

// Free function prototypes  //////////////////////////////////////////////////

std::shared_ptr<component>& createComponent(component_type c);

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
