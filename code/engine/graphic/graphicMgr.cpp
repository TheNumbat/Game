// Program Information ////////////////////////////////////////////////////////

/**
	@file graphicMgr.cpp

	@brief Implementaiton file for graphicMgr class

	graphicMgr manages all things graphical

	@author Max Slater

	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "graphicMgr.h"

#include <SDL.h>
#include <SDL_image.h>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

/**
	@brief graphicMgr constructor

	Sets up log and defaults everything to null and not good
*/
graphicMgr::graphicMgr()
{
	sdl_renderer = NULL;
	sdl_window = NULL;
	good = false;
	logger.StartLog("GRAPHICS");
}

/**
	@brief graphicMgr destructor

	Frees all textures, closes down SDL graphics and window
*/
graphicMgr::~graphicMgr()
{
	kill();
}

/**
	@brief Initializes SDL window and graphics and image system

	@param[in] winTitle title of SDL window
	@param[in] sw initial screen width
	@param[in] sh initial screen height

	@return success

	@exception couldn't initialize sdl video - return false with no change
	@exception couldn't initialize image system - return false with no change
	@exception couldn't create sdl window - return false with no change
	@exception couldn't create sdl renderer - return false with no change
*/
bool graphicMgr::init(const std::string& winTitle, uint32 sw, uint32 sh)
{
	if( good )
	{
		logger.LogWarn("Trying to reintialize graphics system");
		return false;
	}

	// Init video
	bool result = SDL_InitSubSystem(SDL_INIT_VIDEO) == 0;
	assert(result);
	if( !result )
	{
		logger.LogFatal((std::string)"Failed to intialize sdl video subsystem! SDL Error: " + SDL_GetError());
		return false;
	}

	// Init images
	result = IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);
	assert(result);
	if( !result )
	{
		logger.LogFatal((std::string)"Failed to intialize sdl image system! SDL Error: " + SDL_GetError());
		return false;	
	}

	// Create window
	sdl_window = SDL_CreateWindow(winTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sw, sh, SDL_WINDOW_SHOWN);
	assert(sdl_window);
	if( !sdl_window )
	{
		logger.LogFatal((std::string)"Failed to create SDL window! SDL Error: " + SDL_GetError());
		return false;
	}

	// Create renderer
	sdl_renderer = SDL_CreateRenderer((SDL_Window*)sdl_window, -1, SDL_RENDERER_ACCELERATED);
	assert(sdl_renderer);
	if( !sdl_renderer )
	{
		logger.LogFatal((std::string)"Failed to create SDL accelerated renderer! SDL Error: " + SDL_GetError());
		SDL_DestroyWindow((SDL_Window*)sdl_window);
		sdl_window = NULL;
		return false;
	}

	logger.LogInfo("Graphics initialized");

	// Success
	good = true;
	return true;
}

/**
	@brief Closes SDL window and graphics and image system

	@return success

	@exception already dead - return false with no change
*/
bool graphicMgr::kill()
{
	if( !good )
	{
		logger.LogWarn("Trying to kill dead graphics");
		return false;
	}

	SDL_DestroyRenderer((SDL_Renderer*)sdl_renderer);
	SDL_DestroyWindow((SDL_Window*)sdl_window);

	sdl_renderer = NULL;
	sdl_window = NULL;

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	IMG_Quit();

	logger.LogInfo("Graphics uninitialized");

	good = false;
	return true;
}

bool graphicMgr::displayFrame(bool clearAfter)
{
	return false;
}

bool graphicMgr::loadTexture(const std::string& path, const std::string& ID)
{
	return false;
}

bool graphicMgr::loadTextureRec(const std::string& path)
{
	return false;
}

bool graphicMgr::freeTexture(const std::string& ID)
{
	return false;
}

bool graphicMgr::renderTexture(const std::string& ID, const rect2<int32>& dest_rect)
{
	return false;
}

// Terminating precompiler directives  ////////////////////////////////////////
