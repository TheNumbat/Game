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

#include "vector.h"
#include <string>
#include <memory>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Abtracts an SDL image

	Holds data for an sdl texture, and can load itself

	Used by graphicMgr.
*/
class texture
{
	texture();
	~texture();

	bool load(const std::string& path, void* sdl_renderer);
	bool free();

	void* sdl_texture;
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
