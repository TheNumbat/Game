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
#include "vect.h"
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
	/// type of component
	component_type type;
};

/**
	@brief Position component

	Holds a position
*/
struct component_position : public component
{
	/**
		@brief component_position constructor

		@param[in] _pos initial position
	*/
	ENGINE_API component_position(const map_position& _pos = map_position(0,0,0,0,0,0));

	/**
		@brief component_position destructor

		Does nothing
	*/
	ENGINE_API ~component_position();

	/// position
	map_position pos;
};

/**
	@brief Movement component

	Holds velocity and acceleration data
*/
struct component_movement : public component
{
	/**
		@brief component_movement constructor

		@param[in] vel initial velocity
		@param[in] acc initial acceleration
	*/
	ENGINE_API component_movement(const v2<real32>& vel = v2<real32>(0,0), const v2<real32>& acc = v2<real32>(0,0));

	/**
		@brief component_movement destructor

		Does nothing
	*/
	ENGINE_API ~component_movement();

	/// velocity
	v2<real32> velocity;
	/// acceleration
	v2<real32> acceleration;
};

/**
	@brief Texture component

	Holds texture data 

	@todo animation
*/
struct component_texture : public component
{
	/**
		@brief component_texture constructor

		@param[in] top force texture to render on top
		@param[in] bot force texture to render on bottom

		@exception if both top and bot are set, neither will be set
	*/
	ENGINE_API component_texture(bool top = false, bool bot = false);

	/**
		@brief component_texture destructor

		Does nothing
	*/
	ENGINE_API ~component_texture();

	/**
		@brief adds a texture to the texture component

		@param[in] ID of texture to use
		@param[in] pos position of TLC of texture relative to entity position in meters
		@param[in] dim dimension of texture in meters

		@return success (always true)
	*/
	ENGINE_API bool addTexture(const std::string& ID, const v2<real32>& pos, const v2<real32>& dim);

	/**
		@brief removes a texture from the component

		@param[in] ID of texture to remove

		@return success

		@exception ID not found, returns false
	*/
	ENGINE_API bool removeTexture(const std::string& ID);

	/// IDs of textures
	std::vector<std::string> textureIDs;
	/// positions of textures (in meters)
	std::vector<v2<real32>> texturePositions;
	/// dimensions of textures (in meters)
	std::vector<v2<real32>> textureDimensions;
	/// force render on top
	bool forceTop;
	/// force render on bottom
	bool forceBot;
};

// Free function prototypes  //////////////////////////////////////////////////

/**
	@brief creates a component of specified type, returns a polymorphic pointer

	@param[in] c type of component to create

	@return pointer to created component
*/
std::shared_ptr<component> createComponent(component_type c);

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
