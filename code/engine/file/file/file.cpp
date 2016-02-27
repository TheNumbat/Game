// Program Information ////////////////////////////////////////////////////////

/**
	@file file.cpp

	@brief Implementaiton file for file class

	Stores position of camera, TLC of screen.

	@author Max Slater

	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "file.h"

#include <SDL.h>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

file::file()
{
	sdl_file = NULL;
	good = false;
}

file::~file()
{
	free();
}

bool file::load(const std::string& path, int8 type, const std::string& access)
{
	free();

	std::string fullAccess = access;
	fullAccess += type;

	// Load image file
	sdl_file = SDL_RWFromFile(path.c_str(),fullAccess.c_str());
	if(!sdl_file)
	{
		return false;
	}

	// Success
	good = true;
	return true;
}

bool file::free()
{
	if(good)
	{
		SDL_RWclose((SDL_RWops*)sdl_file);
		sdl_file = NULL;

		good = false;
		return true;
	}
	return false;
}

// Terminating precompiler directives  ////////////////////////////////////////
