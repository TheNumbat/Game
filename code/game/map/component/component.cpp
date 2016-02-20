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
#include <algorithm>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

std::shared_ptr<component> createComponent(component_type c)
{
	switch(c)
	{
		case ctype_position:
			return std::static_pointer_cast<component>(std::make_shared<component_position>());
			break;
		case ctype_movement:
			return std::static_pointer_cast<component>(std::make_shared<component_movement>());
			break;
		case ctype_texture:
			return std::static_pointer_cast<component>(std::make_shared<component_texture>());
			break;
	}
}

// Class/Data Structure member implementations  ///////////////////////////////

component_position::component_position(const map_position& _pos)
{
	type = ctype_position;
	position = _pos;
}

component_position::~component_position()
{

}

component_movement::component_movement(const v2<real32>& vel, const v2<real32>& acc)
{
	velocity = vel;
	acceleration = acc;
}

component_movement::~component_movement()
{

}

component_texture::component_texture(bool top, bool bot)
{
	if(!(top && bot))
	{
		forceTop = top;
		forceBot = bot;
	}
}

component_texture::~component_texture()
{

}

bool component_texture::addTexture(const std::string& ID, const v2<real32>& pos, const v2<real32>& dim)
{
	textureIDs.push_back(ID);
	texturePositions.push_back(pos);
	textureDimensions.push_back(dim);

	return true;
}

bool component_texture::removeTexture(const std::string& ID)
{
	auto texture = std::find(textureIDs.begin(),textureIDs.end(),ID);
	if(texture == textureIDs.end())
	{
		return false;
	}

	int position = texture - textureIDs.begin();

	textureIDs.erase(textureIDs.begin() + position);
	texturePositions.erase(texturePositions.begin() + position);
	textureDimensions.erase(textureDimensions.begin() + position);

	return true;
}

// Terminating precompiler directives  ////////////////////////////////////////
