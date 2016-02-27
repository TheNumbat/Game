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

		@exception SDL_Image not initialized, will fail and return false
		@exception Unable to load image, will fail and return false
		@exception Unable to create texture, will fail and return false
		@exception Unable to set blend mode, will fail and return false
	*/
	bool load(const std::string& path, void* sdl_renderer, blendmode b = blend_alpha);

	/**
		@brief changes the blend mode

		@param[in] b blend mode to use

		@return success

		@exception Unable to set blend mode, will fail and return false
	*/
	bool setBlendmode(blendmode b = blend_alpha);

	/**
		@brief Frees texture

		Deallocates texture

		@return success

		@exception if texture is already freed does nothing, returns false
	*/
	bool free();

	void* sdl_texture;
	blendmode blend;
	bool good;

	friend class graphicMgr;

	/// @note Friendship for smart pointers
	friend class std::default_delete<texture>;
	friend std::unique_ptr<texture> std::make_unique<texture>();
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
