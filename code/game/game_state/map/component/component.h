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

#include "game_state\map\position\position.h"

#include <engine_state.h>
#include <rect.h>
#include <vect.h>
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

		@exception if both top and bot are set, neither will be set
	*/
	component_texture();

	/**
		@brief component_texture destructor

		Does nothing
	*/
	~component_texture();

	/**
		@brief adds a texture to the texture component

		@param[in] ID of texture sub-component
		@param[in] texID ID of texture to use
		@param[in] dstRect position and size of texture (relative to entity position) in meters
		@param[in] srcRect rectange to pull from texture in pixels
		@param[in] layer of texture to render
		@param[in] b blend mode of texture
		@param[in] c color mod of texture
		@param[in] rotPoint point around which to rotate in meters
		@param[in] rot how much to rotate in degrees
		@param[in] flip how to flip the texture 

		@return success (always true)
	*/
	bool addTexture(const std::string& ID, const std::string& texID, const rect2<real32>& dstRect, 
					const rect2<int32>& srcRect = rect2<int32>(0,0,0,0), int32 layer = 0, blendmode b = blend_alpha, 
					color c = color(255,255,255,0), const v2<real32>& rotPoint = v2<real32>(0,0), real32 rot = 0,
					uint32 flip = 0);

	/**
		@brief removes a texture from the component

		@param[in] ID of texture to remove

		@return success

		@exception ID not found, returns false
	*/
	bool removeTexture(const std::string& ID);

	struct sub_texture
	{
		std::string texID;
		rect2<real32> texRect;
		blendmode blend;
		color mod;
		int32 layer;

		rect2<int32> srcPixelRect;
		v2<real32> rotPt;
		real32 rot;
		uint32 flip;
	};

	std::vector<std::string> IDs;
	std::vector<sub_texture> textures;
};

/**
	@brief Text texture component

	Holds text render data 
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
		@param[in] message text to render
		@param[in] dstRect position and size of texture (relative to entity position) in meters
		@param[in] srcRect rectange to pull from texture in pixels
		@param[in] layer of texture to render
		@param[in] b blend mode of text_texture
		@param[in] c color mod of text_texture
		@param[in] rotPoint point around which to rotate in meters
		@param[in] rot how much to rotate in degrees
		@param[in] flip how to flip the texture 

		@return success (always true)
	*/
	bool addText(const std::string& ID, const std::string& fontID, const std::string& message, const rect2<real32>& dstRect, 
				 const rect2<int32>& srcRect = rect2<int32>(0,0,0,0), int32 layer = 0, blendmode b = blend_alpha, 
				 color c = color(255,255,255,0), const v2<real32>& rotPoint = v2<real32>(0,0), real32 rot = 0,
				 uint32 flip = 0);

	/**
		@brief removes a text_texture from the component

		@param[in] ID of text_texture to remove

		@return success

		@exception ID not found, returns false
	*/
	bool removeText(const std::string& ID);

	struct sub_text_texture
	{
		std::string fontID;
		std::string message;
		rect2<real32> texRect;
		rect2<int32> srcPixelRect;
		blendmode blend;
		color mod;
		int32 layer;

		v2<real32> rotPt;
		real32 rot;
		uint32 flip;
	};

	std::vector<std::string> IDs;
	std::vector<sub_text_texture> textures;
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
