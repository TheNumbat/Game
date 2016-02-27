// Program Information ////////////////////////////////////////////////////////

/**
	@file font.cpp

	@brief Implementaiton file for font class

	Stores a true type font

	@author Max Slater

	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "font.h"

#include <SDL_ttf.h>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

font::font()
{
	sdl_font = NULL;
	good = false;
}

font::~font()
{
	free();
}

bool font::load(const std::string& path, int size)
{
	free();

	// Load font file
	sdl_font = TTF_OpenFont(path.c_str(), size);
	if(!sdl_font)
	{
		return false;
	}

	// Success
	good = true;
	return true;
}

bool font::free()
{
	if(good)
	{
		TTF_CloseFont((TTF_Font*)sdl_font);
		sdl_font = NULL;

		good = false;
		return true;
	}
	return false;
}

// Terminating precompiler directives  ////////////////////////////////////////
