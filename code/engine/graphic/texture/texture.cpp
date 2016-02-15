// Program Information ////////////////////////////////////////////////////////

/**
	@file texture.cpp

	@brief Implementaiton file for texture class

	Stores position of camera, TLC of screen.

	@author Max Slater

	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "texture.h"

#include <SDL_image.h>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

texture::texture()
{
	sdl_texture = NULL;
	good = false;
}

texture::~texture()
{
	free();
}

bool texture::load(const std::string& path, void* sdl_renderer)
{
	free();

	// Load image file
	SDL_Surface* temp = IMG_Load(path.c_str());
	assert(temp);
	if(!temp)
	{
		return false;
	}

	// Create texture
	sdl_texture = SDL_CreateTextureFromSurface((SDL_Renderer*)sdl_renderer, temp);
	SDL_FreeSurface(temp);

	assert(sdl_texture);
	if(!sdl_texture)
	{
		return false;
	}

	// Success
	good = true;
	return true;
}

bool texture::free()
{
	if(good)
	{
		SDL_DestroyTexture((SDL_Texture*)sdl_texture);
		sdl_texture = NULL;

		good = false;
		return true;
	}
	return false;
}

// Terminating precompiler directives  ////////////////////////////////////////
