// Program Information ////////////////////////////////////////////////////////

/**
	@file font.h

	@brief Declartion file for font class

	Stores a true type font

	@author Max Slater

	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include <string>
#include <memory>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Abtracts a true type font

	Used by graphicMgr.
*/
class font
{
	/**
		@brief font constructor

		Sets font to uninititialized
	*/
	font();

	/**
		@brief font destructor

		Frees the font, calls free()
	*/
	~font();

	/**
		@brief loads font

		Loads image file using SDL_Image

		@param[in] path is the file path of the true type font file
		@param[in] s the font size to use

		@return success

		@exception Unable to load font, will fail and return false
	*/
	bool load(const std::string& path, int32 s);

	/**
		@brief Frees font

		Deallocates font

		@return success

		@exception if font is already freed does nothing, returns false
	*/
	bool free();

	/// Pointer to SDL_ttf structure
	void* sdl_font;
	/// Font size
	int32 size;
	/// If font is good
	bool good;

	friend class graphicMgr;

	/// Friendship for smart pointers
	friend class std::default_delete<font>;
	/// Friendship for smart pointers
	friend std::unique_ptr<font> std::make_unique<font>();
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
