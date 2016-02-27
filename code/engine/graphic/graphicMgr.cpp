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
#include <SDL_ttf.h>
#include <dirent.h>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

graphicMgr::graphicMgr()
{
	sdl_renderer = NULL;
	sdl_window = NULL;
	good = false;
	logger.StartLog("GRAPHICS");
}

graphicMgr::~graphicMgr()
{
	textures.clear();
	fonts.clear();
	kill();
}

bool graphicMgr::init(const std::string& winTitle, uint32 sw, uint32 sh)
{
	if(good)
	{
		logger.LogWarn("Trying to reintialize graphics system");
		return false;
	}

	// Init video
	bool result = SDL_InitSubSystem(SDL_INIT_VIDEO) == 0;
	if(!result)
	{
		logger.LogFatal((std::string)"Failed to intialize sdl video subsystem! SDL Error: " + SDL_GetError());
		return false;
	}

	// Init images
	result = IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);
	if(!result)
	{
		logger.LogFatal((std::string)"Failed to intialize sdl image system! IMG Error: " + IMG_GetError());
		return false;	
	}

	// Init fonts
	result = TTF_Init();
	if(result)
	{
		logger.LogFatal((std::string)"Failed to intialize sdl ttf font system! TTF Error: " + TTF_GetError());
		return false;	
	}

	// Create window
	sdl_window = SDL_CreateWindow(winTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sw, sh, SDL_WINDOW_SHOWN);
	if(!sdl_window)
	{
		logger.LogFatal((std::string)"Failed to create SDL window! SDL Error: " + SDL_GetError());
		return false;
	}

	// Create renderer
	sdl_renderer = SDL_CreateRenderer((SDL_Window*)sdl_window, -1, SDL_RENDERER_ACCELERATED);
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
	TTF_Quit();

	logger.LogInfo("Graphics uninitialized");

	// Success
	good = false;
	return true;
}

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

bool graphicMgr::getWinDim(int32& w, int32& h)
{
	// Test window
	if(!sdl_window)
	{
		logger.LogWarn("Cannot get window dimension, window does not exist!");
		return false;
	}

	// Get window size
	SDL_GetWindowSize((SDL_Window*)sdl_window,&w,&h);
	return true;
}

bool graphicMgr::addTextTexture(const std::string& texID, const std::string& fontID, const std::string& text, v4<uint8> color, blendmode b)
{
	if(textures.find(texID) != textures.end())
	{
		logger.LogWarn("Texture ID " + texID + " already taken!");
		return false;
	}

	auto fontEntry = fonts.find(fontID);
	if(fontEntry == fonts.end())
	{
		logger.LogWarn("Font ID " + fontID + " could not be found!");
		return false;
	}

	SDL_Color sdl_color;
	sdl_color.r = color.r;
	sdl_color.g = color.g;
	sdl_color.b = color.b;
	sdl_color.a = color.a;

	/// @todo more text rendering options
	SDL_Surface* textSurface = TTF_RenderText_Solid((TTF_Font*)fontEntry->second->sdl_font,text.c_str(),sdl_color);

	if(!textSurface)
	{
		logger.LogWarn("Failed to create surface from font ID " + fontID + " TTF_Error: " + TTF_GetError());
		return false;
	}

	std::unique_ptr<texture> newTexture = std::make_unique<texture>();
	if(!newTexture->load(textSurface,sdl_renderer,b))
	{
		SDL_FreeSurface(textSurface);
		logger.LogWarn((std::string)"Failed to load texture from font surface, TTF_Error: " + TTF_GetError());
		return false;
	}

	SDL_FreeSurface(textSurface);
	logger.LogInfo("Created texture ID " + texID + " using font ID " + fontID);
	textures.insert({texID,std::move(newTexture)});
	return true;
}

bool graphicMgr::loadTexture(const std::string& path, const std::string& ID, blendmode b)
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
	if(!newTexture->load(path,sdl_renderer,b))
	{
		logger.LogWarn("Failed to load texture ID: " + texID + " from " + path + " Img_error: " + IMG_GetError() + (std::string)" SDL_error: " + SDL_GetError());
		return false;
	}

	// Success
	textures.insert({texID,std::move(newTexture)});
	logger.LogInfo("Loaded texture ID: " + texID + " from " + path);
	return true;
}

bool graphicMgr::loadFont(const std::string& path, const std::string& ID, int size)
{
	if(fonts.find(ID) != fonts.end())
	{
		logger.LogWarn("Font ID " + ID + " already taken!");
		return false;
	}

	std::unique_ptr<font> newFont = std::make_unique<font>();
	std::string fontID;

	if(!ID.size())
	{
		fontID = path.substr(path.find_last_of("/\\") + 1, path.find_last_of(".") - path.find_last_of("/\\") - 1);
	}
	else
	{
		fontID = ID;
	}

	// Load
	if(!newFont->load(path,size))
	{
		logger.LogWarn("Failed to load font ID: " + fontID + " from " + path + " TTF_error: " + TTF_GetError());
		return false;
	}

	// Success
	fonts.insert({fontID,std::move(newFont)});
	logger.LogInfo("Loaded font ID: " + fontID + " from " + path);
	return true;
}

bool graphicMgr::setBlendmode(const std::string& ID, blendmode b)
{
	auto textureItem = textures.find(ID);

	if(textureItem == textures.end())
	{
		logger.LogWarn("Can't set texture blend mode, could not find loaded texture with ID: " + ID);
		return false;
	}

	if(!textureItem->second->setBlendmode(b))
	{
		logger.LogWarn("Failed to set texture blend mode ID: " + ID + " SDL_error: " + SDL_GetError());
		return false;
	}

	return true;
}

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

bool graphicMgr::loadFontRec(const std::string& path)
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
	if(!directory)
	{
		logger.LogWarn("Failed to open font directory at " + dirPath);
		return false;
	}

	logger.LogInfo("Loading font directory at " + dirPath);
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
				loadFont(dirPath + entryName);
			}
			else
			{
				loadFontRec(dirPath + entryName + '/');
			}
		}
	}

	logger.ExitSec();

	// Success
	return true;
}

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

bool graphicMgr::freeFont(const std::string& ID)
{
	auto fontEntry = textures.find(ID);
	if(fontEntry == textures.end())
	{
		logger.LogWarn("Can't free font ID: " + ID + ", could not find");
		return false;
	}

	textures.erase(fontEntry);

	// Success
	logger.LogInfo("Freed font ID: " + ID);
	return true;
}

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

bool graphicMgr::renderTextureEx(const std::string& ID, const rect2<int32>& dest_rect,
 								 const rect2<int32>& src_rect, const v2<int32>& rot_point,
					 			 real32 rotation, uint32 flip)
{
	if(!good)
	{
		logger.LogWarn("Can't display texture, graphics not initialized");
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
	if(!result)
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
