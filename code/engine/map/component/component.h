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
#include "vector.h"
#include <memory>
#include <vector>
#include <string>

// Global constant definitions  ///////////////////////////////////////////////

/**
	@brief Holds types of components

	Type value for each type of position
*/
enum component_type
{
	ctype_position = 0,
	ctype_movement = 1,
	ctype_texture = 2
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
	ENGINE_API component_position(const map_position& _pos = map_position(0,0,0,0,0,0));
	ENGINE_API ~component_position();

	map_position pos;
};

/**
	@brief Movement component

	Holds velocity and acceleration data
*/
struct component_movement : public component
{
	ENGINE_API component_movement(const v2<real32>& vel = v2<real32>(0,0), const v2<real32>& acc = v2<real32>(0,0));
	ENGINE_API ~component_movement();

	v2<real32> velocity;
	v2<real32> acceleration;
};

/**
	@brief Texture component

	Holds texture data 

	@todo animation
*/
struct component_texture : public component
{
	ENGINE_API component_texture(bool top = false, bool bot = false);
	ENGINE_API ~component_texture();

	ENGINE_API bool addTexture(const std::string& ID, const v2<real32>& pos, const v2<real32>& dim);
	ENGINE_API bool removeTexture(const std::string& ID);

	std::vector<std::string> textureIDs;
	std::vector<v2<real32>> texturePositions;
	std::vector<v2<real32>> textureDimensions;
	bool forceTop;
	bool forceBot;
};

// Free function prototypes  //////////////////////////////////////////////////

const std::shared_ptr<component>& createComponent(component_type c);

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
