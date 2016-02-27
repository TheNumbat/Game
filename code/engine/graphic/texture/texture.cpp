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
	blend = blend_none;
}

texture::~texture()
{
	free();
}

bool texture::load(const std::string& path, void* sdl_renderer, blendmode b)
{
	free();

	// Load image file
	SDL_Surface* temp = IMG_Load(path.c_str());
	if(!temp)
	{
		return false;
	}

	// Create texture
	sdl_texture = SDL_CreateTextureFromSurface((SDL_Renderer*)sdl_renderer, temp);
	SDL_FreeSurface(temp);

	if(!sdl_texture)
	{
		return false;
	}

	if(!setBlendmode(b))
	{
		free();
		return false;
	}

	// Success
	good = true;
	return true;
}

bool texture::load(void* sdl_surface, void* sdl_renderer, blendmode b)
{
	free();

	sdl_texture = SDL_CreateTextureFromSurface((SDL_Renderer*)sdl_renderer, (SDL_Surface*)sdl_surface);
	if(!sdl_texture)
	{
		return false;
	}

	if(!setBlendmode(b))
	{
		free();
		return false;
	}

	// Success
	good = true;
	return true;
}

bool texture::setBlendmode(blendmode b)
{
	int result;
	switch(b)
	{
		case blend_alpha:
			result = SDL_SetTextureBlendMode((SDL_Texture*)sdl_texture,SDL_BLENDMODE_BLEND);
			break;
		case blend_additive:
			result = SDL_SetTextureBlendMode((SDL_Texture*)sdl_texture,SDL_BLENDMODE_ADD);
			break;
		case blend_modulate:
			result = SDL_SetTextureBlendMode((SDL_Texture*)sdl_texture,SDL_BLENDMODE_MOD);
			break;
		default: // or blend_none
			result = SDL_SetTextureBlendMode((SDL_Texture*)sdl_texture,SDL_BLENDMODE_NONE);
			break;
	}

	if(result != 0)
	{
		return false;
	}

	return true;
}

bool texture::setColorMod(color c)
{
	int result = SDL_SetTextureColorMod((SDL_Texture*)sdl_texture,c.r,c.g,c.b);
	return result == 0;
}

bool texture::free()
{
	if(good)
	{
		SDL_DestroyTexture((SDL_Texture*)sdl_texture);
		sdl_texture = NULL;

		blend = blend_none;
		good = false;
		return true;
	}
	return false;
}

// Terminating precompiler directives  ////////////////////////////////////////
