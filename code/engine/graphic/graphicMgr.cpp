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
#include <dirent.h>

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
	if(good)
	{
		logger.LogWarn("Trying to reintialize graphics system");
		return false;
	}

	// Init video
	bool result = SDL_InitSubSystem(SDL_INIT_VIDEO) == 0;
	assert(result);
	if(!result)
	{
		logger.LogFatal((std::string)"Failed to intialize sdl video subsystem! SDL Error: " + SDL_GetError());
		return false;
	}

	// Init images
	result = IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);
	assert(result);
	if(!result)
	{
		logger.LogFatal((std::string)"Failed to intialize sdl image system! SDL Error: " + SDL_GetError());
		return false;	
	}

	// Create window
	sdl_window = SDL_CreateWindow(winTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sw, sh, SDL_WINDOW_SHOWN);
	assert(sdl_window);
	if(!sdl_window)
	{
		logger.LogFatal((std::string)"Failed to create SDL window! SDL Error: " + SDL_GetError());
		return false;
	}

	// Create renderer
	sdl_renderer = SDL_CreateRenderer((SDL_Window*)sdl_window, -1, SDL_RENDERER_ACCELERATED);
	assert(sdl_renderer);
	if(!sdl_renderer)
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
	if(!good)
	{
		logger.LogWarn("Trying to kill dead graphics");
		return false;
	}

	// Destory SDL stuff
	SDL_DestroyRenderer((SDL_Renderer*)sdl_renderer);
	SDL_DestroyWindow((SDL_Window*)sdl_window);

	sdl_renderer = NULL;
	sdl_window = NULL;

	// Quit
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	IMG_Quit();

	logger.LogInfo("Graphics uninitialized");

	// Success
	good = false;
	return true;
}

/**
	@brief Renders backbuffer to screen

	May also clear the buffer afterward

	@param[in] clearAfter whether or not to clear the buffer after it is rendered

	@return success
*/
bool graphicMgr::displayFrame(bool clearAfter)
{
	if(!good)
	{
		logger.LogWarn("Cannot display frame, graphics not initialized");
		return false;
	}

	// Swap buffers
	SDL_RenderPresent((SDL_Renderer*)sdl_renderer);

	#ifdef VERBOSE_GRAPHICS
		logger.LogInfo("Displayed frame");
	#endif

	// Clear backbuffer
	if(clearAfter)
	{
		bool result = SDL_RenderClear((SDL_Renderer*)sdl_renderer);
		assert(!result);
		if(result)
		{
			logger.LogWarn((std::string)"Failed to clear renderer. SDL Error: " + SDL_GetError());
			return false;
		}

		#ifdef VERBOSE_GRAPHICS
			logger.LogInfo("Cleared renderer");
		#endif
	}

	// Success
	return true;
}

/**
	@brief Loads a texture from file into texture map

	@param[in] path file path of image file
	@param[in] ID of texture to load, if empty will use file name

	@return success

	@exception couldn't load texture, does nothing, returns false
*/
bool graphicMgr::loadTexture(const std::string& path, const std::string& ID)
{
	if(textures.find(ID) != textures.end())
	{
		logger.LogWarn("Texture ID " + ID + " already taken!");
		return false;
	}

	std::unique_ptr<texture> newTexture = std::make_unique<texture>();
	std::string texID;

	if(!ID.size())
	{
		texID = path.substr(path.find_last_of("/\\") + 1, path.find_last_of(".") - path.find_last_of("/\\") - 1);
	}
	else
	{
		texID = ID;
	}

	// Load
	if(!newTexture->load(path,sdl_renderer))
	{
		logger.LogWarn("Failed to load texture ID: " + texID + " from " + path + " Img_error: " + IMG_GetError());
		return false;
	}

	// Success
	textures.insert({texID,std::move(newTexture)});
	logger.LogInfo("Loaded texture ID: " + texID + " from " + path);
	return true;
}

/**
	@brief Loads all image files from a folder (and subfolders) into textute map

	Will just use file name for texture IDs

	@param[in] path file path of image file

	@note uses dirent.h

	@return success -- always true
*/
bool graphicMgr::loadTextureRec(const std::string& path)
{
	DIR *directory;
	struct dirent *entry;
	std::string dirPath = path;

	if(dirPath.back() != '/' && dirPath.back() != '\\')
	{
		dirPath.append("/");
	} 

	// Open directory
	directory = opendir(dirPath.c_str());
	assert(directory);
	if(!directory)
	{
		logger.LogWarn("Failed to open texture directory at " + dirPath);
		return false;
	}

	logger.LogInfo("Loading texture directory at " + dirPath);
	logger.EnterSec();

	// Read directory
	while(entry = readdir(directory))
	{
		std::string entryName = entry->d_name;

		if(entryName != ".." && entryName != ".")
		{
			// Load texture or load another directory
			/// @todo make this better, it just tests for a .extension
			if(entryName[entryName.size() - 4] == '.')
			{
				loadTexture(dirPath + entryName);
			}
			else
			{
				loadTextureRec(dirPath + entryName + '/');
			}
		}
	}

	logger.ExitSec();

	// Success
	return true;
}

/**
	@brief Unloads a texture from the texture map

	@param[in] ID of texture to unload

	@return success

	@exception couldn't find texture, no change, return false
*/
bool graphicMgr::freeTexture(const std::string& ID)
{
	auto tex = textures.find(ID);
	if(tex == textures.end())
	{
		logger.LogWarn("Can't free texture ID: " + ID + ", could not find");
		return false;
	}

	textures.erase(tex);

	// Success
	logger.LogInfo("Freed texture ID: " + ID);
	return true;
}

/**
	@brief Renders a texture to the backbuffer

	Only renders an entire texture into destination rect, use renderTextureEx 
	for more options.

	@param[in] ID of texture to render
	@param[in] dest_rect rectangle on the backbuffer to render into

	@return success

	@exception could not find texture, returns false
	@exception could not render texture, returns false
	@exception graphics not intialized, returns false
*/
bool graphicMgr::renderTexture(const std::string& ID, const rect2<int32>& dest_rect)
{
	if(!good)
	{
		logger.LogWarn("Can't display texture, graphics log initialized");
		return false;
	}

	auto textureItem = textures.find(ID);

	if(textureItem == textures.end())
	{
		logger.LogWarn("Can't display texture, could not find loaded texture with ID: " + ID);
		return false;
	}

	SDL_Rect sdl_dest_rect;
	sdl_dest_rect.x = dest_rect.x;
	sdl_dest_rect.y = dest_rect.y;
	sdl_dest_rect.w = dest_rect.w;
	sdl_dest_rect.h = dest_rect.h;

	bool result = SDL_RenderCopy((SDL_Renderer*)sdl_renderer,(SDL_Texture*)textureItem->second->sdl_texture,NULL,&sdl_dest_rect);
	assert(!result);
	if(result)
	{
		logger.LogWarn("Failed to Render texture ID: " + ID + ", SDL_Error: " + SDL_GetError());
		return false;
	}

	#ifdef VERBOSE_GRAPHICS
		logger.LogInfo("Rendered texture ID: " + ID);
	#endif

	return true;
}

/**
	@brief Renders a texture to the backbuffer

	Renders texture with more options

	@param[in] ID of texture to render
	@param[in] dest_rect rectangle on the backbuffer to render into
	@param[in] src_rect rectangle to take from the texture to render
	@param[in] rot_point point to rotate texture around
	@param[in] rotation amount (in degrees)
	@param[in] flip horizontal/verticle flip (use constants in graphicMgr.h)

	@return success

	@exception could not find texture, returns false
	@exception could not render texture, returns false
	@exception graphics not intialized, returns false
*/
bool graphicMgr::renderTextureEx(const std::string& ID, const rect2<int32>& dest_rect,
 								 const rect2<int32>& src_rect, const v2<int32>& rot_point,
					 			 real32 rotation, uint32 flip)
{
	if(!good)
	{
		logger.LogWarn("Can't display texture, graphics log initialized");
		return false;
	}

	auto textureItem = textures.find(ID);

	if(textureItem == textures.end())
	{
		logger.LogWarn("Can't display texture, could not find loaded texture with ID: " + ID);
		return false;
	}

	SDL_Rect sdl_dest_rect;
	sdl_dest_rect.x = dest_rect.x;
	sdl_dest_rect.y = dest_rect.y;
	sdl_dest_rect.w = dest_rect.w;
	sdl_dest_rect.h = dest_rect.h;

	SDL_Rect sdl_src_rect;
	sdl_src_rect.x = src_rect.x;
	sdl_src_rect.y = src_rect.y;
	sdl_src_rect.w = src_rect.w;
	sdl_src_rect.h = src_rect.h;

	SDL_Point sdl_rot_point;
	sdl_rot_point.x = rot_point.x;
	sdl_rot_point.y = rot_point.y;

	SDL_RendererFlip sdl_flip = SDL_FLIP_NONE;
	if(flip & FLIP_HORZ && flip & FLIP_VERT)
	{
		sdl_flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	}
	else if(flip & FLIP_HORZ)
	{
		sdl_flip = (SDL_RendererFlip)SDL_FLIP_HORIZONTAL;
	}
	else if(flip & FLIP_VERT)
	{
		sdl_flip = (SDL_RendererFlip)SDL_FLIP_VERTICAL;
	}

	bool result = SDL_RenderCopyEx((SDL_Renderer*)sdl_renderer,(SDL_Texture*)textureItem->second->sdl_texture,
								   &sdl_src_rect,&sdl_dest_rect,rotation,&sdl_rot_point,sdl_flip);
	assert(!result);
	if(result)
	{
		logger.LogWarn("Failed to Render texture ID: " + ID + ", SDL_Error: " + SDL_GetError());
		return false;
	}

	#ifdef VERBOSE_GRAPHICS
		logger.LogInfo("Rendered texture ID: " + ID);
	#endif
		
	return true;
}

// Terminating precompiler directives  ////////////////////////////////////////
