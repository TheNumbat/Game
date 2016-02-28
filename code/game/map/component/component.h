// Program Information ////////////////////////////////////////////////////////

/**
	@file component.h

	@brief Declartion file for component classes

	Components are held within entities

	@author Max Slater

	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "game_common.h"

#include "map\position\position.h"
#include "render/render.h"

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
	ctype_texture = 2,
	ctype_text_texture = 3
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
	component_position(const map_position& _pos = map_position(0,0,0,0,0,0));

	/**
		@brief component_position destructor

		Does nothing
	*/
	~component_position();

	/// position
	map_position position;
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
	component_movement(const v2<real32>& vel = v2<real32>(0,0), const v2<real32>& acc = v2<real32>(0,0));

	/**
		@brief component_movement destructor

		Does nothing
	*/
	~component_movement();

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
	component_texture(bool top = false, bool bot = false);

	/**
		@brief component_texture destructor

		Does nothing
	*/
	~component_texture();

	/**
		@brief adds a texture to the texture component

		@param[in] ID of texture sub-component
		@param[in] texID ID of texture to render
		@param[in] pos position of TLC of texture relative to entity position in meters
		@param[in] dim dimension of texture in meters
		@param[in] b blend mode of texture
		@param[in] c color mod of texture

		@return success (always true)
	*/
	bool addTexture(const std::string& ID, const std::string& texID, const v2<real32>& pos, const v2<real32>& dim, 
					blendmode b = blend_alpha, color c = color(255,255,255,0));

	/**
		@brief removes a texture from the component

		@param[in] ID of texture to remove

		@return success

		@exception ID not found, returns false
	*/
	bool removeTexture(const std::string& ID);

	std::vector<std::string> IDs;
	std::vector<std::string> textureIDs;
	std::vector<v2<real32>> texturePositions;
	std::vector<v2<real32>> textureDimensions;
	std::vector<blendmode> textureBlends;
	std::vector<color> textureMods;
	bool forceTop;
	bool forceBot;
};

/**
	@brief Text texture component

	Holds text render data 

	@note will always render after normal textures, so will be on top
*/
struct component_text_texture : public component
{
	/**
		@brief component_text_texture constructor
	*/
	component_text_texture();

	/**
		@brief component_text_texture destructor
	*/
	~component_text_texture();

	/**
		@brief adds a text_texture to the text_texture component

		@param[in] ID of text_texture sub-component
		@param[in] fontID ID of font to use
		@param[in] pos position of TLC of text_texture relative to entity position in meters
		@param[in] dim dimension of text_texture in meters
		@param[in] b blend mode of text_texture
		@param[in] c color mod of text_texture

		@return success (always true)
	*/
	bool addText(const std::string& ID, const std::string& fontID, const std::string& message, const v2<real32>& pos, const v2<real32>& dim, 
				 blendmode b = blend_alpha, color c = color(255,255,255,0));

	/**
		@brief removes a text_texture from the component

		@param[in] ID of text_texture to remove

		@return success

		@exception ID not found, returns false
	*/
	bool removeText(const std::string& ID);

	std::vector<std::string> IDs;
	std::vector<std::string> fontIDs;
	std::vector<std::string> text;
	std::vector<v2<real32>> textPositions;
	std::vector<v2<real32>> textDimensions;
	std::vector<blendmode> textBlends;
	std::vector<color> textMods;
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
