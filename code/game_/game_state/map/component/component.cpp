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
		case ctype_text_texture:
			return std::static_pointer_cast<component>(std::make_shared<component_text_texture>());
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

component_texture::component_texture()
{

}

component_texture::~component_texture()
{

}

bool component_texture::addTexture(const std::string& ID, const std::string& texID, const rect2<real32>& texRect,
								   const rect2<int32>& srcRect, int32 layer, blendmode b, color c, const v2<real32>& rotPoint,
								   real32 rot, uint32 flip)
{
	if(std::find(IDs.begin(),IDs.end(),ID) != IDs.end())
	{
		return false;
	}

	sub_texture newT;
	newT.texID = texID;
	newT.texRect = texRect;
	newT.srcPixelRect = srcRect;
	newT.layer = layer;
	newT.blend = b;
	newT.mod = c;
	newT.rotPt = rotPoint;
	newT.rot = rot;
	newT.flip = flip;

	IDs.push_back(ID);
	textures.push_back(newT);

	return true;
}

bool component_texture::removeTexture(const std::string& ID)
{
	auto tEntry = std::find(IDs.begin(),IDs.end(),ID);
	if(tEntry == IDs.end())
	{
		return false;
	}

	int32 position = tEntry - IDs.begin();

	IDs.erase(tEntry);
	textures.erase(textures.begin() + position);
	return true;
}

component_text_texture::component_text_texture()
{

}

component_text_texture::~component_text_texture()
{

}

bool component_text_texture::addText(const std::string& ID, const std::string& fontID, const std::string& message, 
									 const rect2<real32>& texRect, const rect2<int32>& srcRect, int32 layer, blendmode b, 
									 color c, const v2<real32>& rotPoint, real32 rot, uint32 flip)
{
	if(std::find(IDs.begin(),IDs.end(),ID) != IDs.end())
	{
		return false;
	}

	sub_text_texture newT;
	newT.fontID = fontID;
	newT.message = message;
	newT.texRect = texRect;
	newT.srcPixelRect = srcRect;
	newT.layer = layer;
	newT.blend = b;
	newT.mod = c;
	newT.rotPt = rotPoint;
	newT.rot = rot;
	newT.flip = flip;

	IDs.push_back(ID);
	textures.push_back(newT);

	return true;
}

bool component_text_texture::removeText(const std::string& ID)
{
	auto textEntry = std::find(IDs.begin(),IDs.end(),ID);
	if(textEntry == IDs.end())
	{
		return false;
	}

	int32 position = textEntry - IDs.begin();

	IDs.erase(textEntry);
	textures.erase(textures.begin() + position);
	return true;
}

// Terminating precompiler directives  ////////////////////////////////////////
