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

bool component_texture::addTexture(const std::string& ID, const std::string& texID, const v2<real32>& pos, const v2<real32>& dim, 
								   bool bot, bool top, blendmode b, color c)
{
	IDs.push_back(ID);
	textureIDs.push_back(texID);
	texturePositions.push_back(pos);
	textureDimensions.push_back(dim);
	forceBot.push_back(bot);
	forceTop.push_back(top);
	textureBlends.push_back(b);
	textureMods.push_back(c);
	return true;
}

bool component_texture::removeTexture(const std::string& ID)
{
	auto texture = std::find(IDs.begin(),IDs.end(),ID);
	if(texture == IDs.end())
	{
		return false;
	}

	int32 position = texture - IDs.begin();

	IDs.erase(texture);
	textureIDs.erase(textureIDs.begin() + position);
	texturePositions.erase(texturePositions.begin() + position);
	textureDimensions.erase(textureDimensions.begin() + position);
	textureBlends.erase(textureBlends.begin() + position);
	textureMods.erase(textureMods.begin() + position);
	return true;
}

component_text_texture::component_text_texture()
{

}

component_text_texture::~component_text_texture()
{

}

bool component_text_texture::addText(const std::string& ID, const std::string& fontID, const std::string& message, const v2<real32>& pos, 
									 const v2<real32>& dim, blendmode b, color c)
{
	IDs.push_back(ID);
	fontIDs.push_back(fontID);
	text.push_back(message);
	textPositions.push_back(pos);
	textDimensions.push_back(dim);
	textBlends.push_back(b);
	textMods.push_back(c);
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
	fontIDs.erase(fontIDs.begin() + position);
	textPositions.erase(textPositions.begin() + position);
	textDimensions.erase(textDimensions.begin() + position);
	textBlends.erase(textBlends.begin() + position);
	textMods.erase(textMods.begin() + position);
	return true;
}

// Terminating precompiler directives  ////////////////////////////////////////
