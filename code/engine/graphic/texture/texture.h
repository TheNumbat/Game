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

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Abtracts an SDL image

	Can also move to an entity and follow an entity.

	Used by graphicMgr.
*/
class texture
{
public:
	texture();
	~texture();

private:
	bool load(const std::string& path, void* sdl_renderer);
	bool free();

	/// @note I don't want to put a logger in here but it would help
	void* sdl_texture;
	bool good;

	friend class graphicMgr;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
