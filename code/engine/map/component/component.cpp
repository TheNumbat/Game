// Program Information ////////////////////////////////////////////////////////

/**
	@file component.cpp

	@brief Implementation of component classes

	Components are held within entities

	@author Max Slater
	
	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "component.h"

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

std::shared_ptr<component>& createComponent(component_type c)
{
	switch(c)
	{
		case ctype_position:
			return std::static_pointer_cast<component>(std::make_shared<component_position>());
			break;
	}
}

// Class/Data Structure member implementations  ///////////////////////////////

/**
	@brief component_position constructor

	@param[in] _pos initial position
*/
component_position::component_position(map_position _pos)
{
	type = ctype_position;
	pos = _pos;
}

/**
	@brief component_position destructor

	Does nothing
*/
component_position::~component_position()
{

}


// Terminating precompiler directives  ////////////////////////////////////////
