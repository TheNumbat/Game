// Program Information ////////////////////////////////////////////////////////

/**
	@file texture.h

	@brief Declartion file for texture class

	Texture stores information needed to display a texture

	@author Max Slater

	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include "vect.h"
#include <string>
#include <memory>

// Global constant definitions  ///////////////////////////////////////////////

/**
	@brief Describes what blend mode to use with a texture

	Used by texture.
*/
enum blendmode
{
	blend_none,
	blend_alpha,
	blend_additive,
	blend_modulate
};

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Abtracts an SDL image

	Holds data for an sdl texture, and can load itself

	Used by graphicMgr.
*/
class texture
{
	/**
		@brief texture constructor

		Sets texture to uninititialized
	*/
	texture();

	/**
		@brief texture destructor

		Frees the texture, calls free()
	*/
	~texture();

	/**
		@brief loads texture

		Loads image file using SDL_Image

		@param[in] path is the file path of the image file
		@param[in] sdl_renderer is the renderer used to create the texture
		@param[in] b blend mode to use for the texture (see blendmode)

		@return success

		@exception Unable to load image, will fail and return false
		@exception Unable to create texture, will fail and return false
		@exception Unable to set blend mode, will fail and return false
	*/
	bool load(const std::string& path, void* sdl_renderer, blendmode b = blend_alpha);

	/**
		@brief loads texture

		Loads image file using a pre-loaded surface. Will not destory the surface.

		@param[in] sdl_surface surface to use
		@param[in] sdl_renderer is the renderer used to create the texture
		@param[in] b blend mode to use for the texture (see blendmode)

		@return success

		@exception Unable to create texture, will fail and return false
		@exception Unable to set blend mode, will fail and return false
	*/
	bool load(void* sdl_surface, void* sdl_renderer, blendmode b = blend_alpha);

	/**
		@brief changes the blend mode

		@param[in] b blend mode to use

		@return success

		@exception Unable to set blend mode, will fail and return false
	*/
	bool setBlendmode(blendmode b = blend_alpha);

	/**
		@brief changes the color of the texture

		The r, b, and b will me multiplied into render operations

		@param[in] c color to modulate with -- alpha does not matter

		@return success

		@exception Unable to set color mod, will fail and return false
	*/
	bool setColorMod(color c);

	/**
		@brief Frees texture

		Deallocates texture

		@return success

		@exception if texture is already freed does nothing, returns false
	*/
	bool free();

	/// Pointer to sdl texture
	void* sdl_texture;
	/// Default blend mode
	blendmode blend;
	/// Texture is good
	bool good;

	friend class graphicMgr;

	/// Friendship for smart pointers
	friend class std::default_delete<texture>;
	/// Friendship for smart pointers
	friend std::unique_ptr<texture> std::make_unique<texture>();
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
