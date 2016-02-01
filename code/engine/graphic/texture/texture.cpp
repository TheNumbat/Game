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

/**
	@brief texture constructor

	Sets texture to uninititialized
*/
texture::texture()
{
	sdl_texture = NULL;
	good = false;
}

/**
	@brief texture destructor

	Frees the texture
*/
texture::~texture()
{
	free();
}

/**
	@brief texture constructor

	Loads image file using SDL_Image

	@param[in] path is the file path of the image file
	@param[in] sdl_renderer is the renderer used to create the texture

	@return success

	@exception SDL_Image not initialized, will fail and return false
	@exception Unable to load image, will fail and return false
	@exception Unable to create texture, will fail and return false

	@todo make this log or return some indication of the error
*/
bool texture::load(const std::string& path, void* sdl_renderer)
{
	free();

	SDL_Surface* temp = IMG_Load(path.c_str());
	assert(temp);
	if(!temp)
	{
		return false;
	}

	sdl_texture = SDL_CreateTextureFromSurface((SDL_Renderer*)sdl_renderer, temp);
	SDL_FreeSurface(temp);

	assert(sdl_texture);
	if(!sdl_texture)
	{
		return false;
	}

	good = true;
	return true;
}

/**
	@brief Frees texture

	Deallocates texture

	@return success

	@exception if texture is already freed does nothing, returns false
*/
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
