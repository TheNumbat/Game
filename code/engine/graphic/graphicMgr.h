// Program Information ////////////////////////////////////////////////////////

/**
	@file graphicMgr.h

	@brief Declaration file for graphicMgr class

	graphicMgr manages all things graphical

	@author Max Slater

	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include "texture\texture.h"
#include "log\logMgr.h"

#include "rect.h"

#include <map>
#include <string>
#include <memory>

// Global constant definitions  ///////////////////////////////////////////////

const uint32 FLIP_NONE = 0;
const uint32 FLIP_HORZ = 1<<0;
const uint32 FLIP_VERT = 1<<1;

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Manages graphical display

	See function docuemtnation
*/
class graphicMgr
{
public:
	/**
		@brief graphicMgr constructor

		Sets up log and defaults everything to null and not good
	*/
	ENGINE_API graphicMgr();

	/**
		@brief graphicMgr destructor

		Frees all textures, closes down SDL graphics and window
	*/
	ENGINE_API ~graphicMgr();

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
	ENGINE_API bool init(const std::string& winTitle, uint32 sw = 1280, uint32 sh = 720);
	
	/**
		@brief Closes SDL window and graphics and image system

		@return success

		@exception already dead - return false with no change
	*/
	ENGINE_API bool kill();
	
	/**
		@brief Renders backbuffer to screen

		May also clear the buffer afterward

		@param[in] clearAfter whether or not to clear the buffer after it is rendered

		@return success
	*/
	ENGINE_API bool displayFrame(bool clearAfter = true);

	/**
		@brief Loads a texture from file into texture map

		@param[in] path file path of image file
		@param[in] ID of texture to load, if empty will use file name

		@return success

		@exception couldn't load texture, does nothing, returns false
	*/
	ENGINE_API bool loadTexture(const std::string& path, const std::string& ID = "");

	/**
		@brief Loads all image files from a folder (and subfolders) into textute map

		Will just use file name for texture IDs

		@param[in] path file path of image file

		@note uses dirent.h

		@return success -- always true
	*/
	ENGINE_API bool loadTextureRec(const std::string& path);

	/**
		@brief Unloads a texture from the texture map

		@param[in] ID of texture to unload

		@return success

		@exception couldn't find texture, no change, return false
	*/
	ENGINE_API bool freeTexture(const std::string& ID);

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
	ENGINE_API bool renderTexture(const std::string& ID, const rect2<int32>& dest_rect);

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
	ENGINE_API bool renderTextureEx(const std::string& ID, const rect2<int32>& dest_rect,
									const rect2<int32>& src_rect, const v2<int32>& rot_point,
									real32 rotation, uint32 flip);

private:
	logMgr logger;

	std::map<std::string,std::unique_ptr<texture>> textures;

	void* sdl_renderer;
	void* sdl_window;
	bool good;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
