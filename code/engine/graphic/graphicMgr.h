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
#include "font\font.h"        
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
		@brief Returns the dimensions of the current window

		@param[out] w width of window
		@param[out] h height of window

		@return success

		@exception window does not exist -- returns false
	*/
	ENGINE_API bool getWinDim(int32& w, int32& h);

	/**
		@brief Loads a texture from file into texture map

		@param[in] path file path of image file
		@param[in] ID of texture to load, if empty will use file name
		@param[in] b blend mode, if emptly will use alpha blending

		@return success

		@exception couldn't load texture, does nothing, returns false
	*/
	ENGINE_API bool loadTexture(const std::string& path, const std::string& ID = "", blendmode b = blend_alpha);

	/**
		@brief Creates a texture from a portion of text and a font

		@param[in] texID ID of new texture
		@param[in] fontID ID of font to use
		@param[in] text to render
		@param[in] color of text to render (RGBA), if empty will use white and 100% alpha
		@param[in] b blend mode, if emptly will use alpha blending

		@return success

		@exception couldn't load texture, does nothing, returns false
	*/
	ENGINE_API bool addTextTexture(const std::string& texID, const std::string& fontID, const std::string& text, const v4<uint8>& color = v4<uint8>(255,255,255,255), blendmode b = blend_alpha);

	/**
		@brief Loads a font from file into font map

		@param[in] path file path of true type font file
		@param[in] ID of font to load, if empty will use file name
		@param[in] size of font, if empty will use 14

		@return success

		@exception couldn't load font, does nothing, returns false
	*/
	ENGINE_API bool loadFont(const std::string& path, const std::string& ID = "", int size = 14);

	/**
		@brief Changes a texture's blend mode

		@param[in] ID of texture to use
		@param[in] b blend mode to set to

		@return success

		@exception couldn't change blend mode, does nothing, returns false
	*/
	ENGINE_API bool setBlendmode(const std::string& ID, blendmode b = blend_alpha);

	/**
		@brief Loads all image files from a folder (and subfolders) into textute map

		Will just use file name for texture IDs

		@param[in] path file path of image file

		@note uses dirent.h

		@return success -- always true
	*/
	ENGINE_API bool loadTextureRec(const std::string& path);

	/**
		@brief Loads all true type font files from a folder (and subfolders) into font map

		Will just use file name for font IDs

		@param[in] path of folder to load

		@note uses dirent.h

		@return success -- always true
	*/
	ENGINE_API bool loadFontRec(const std::string& path);

	/**
		@brief Unloads a texture from the texture map

		@param[in] ID of texture to unload

		@return success

		@exception couldn't find texture, no change, return false
	*/
	ENGINE_API bool freeTexture(const std::string& ID);

	/**
		@brief Unloads a font from the font map

		@param[in] ID of font to unload

		@return success

		@exception couldn't find font, no change, return false
	*/
	ENGINE_API bool freeFont(const std::string& ID);

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
	*/
	ENGINE_API bool renderTextureEx(const std::string& ID, const rect2<int32>& dest_rect,
									const rect2<int32>& src_rect, const v2<int32>& rot_point,
									real32 rotation, uint32 flip);

	/**
		@brief Directly renders a texture -- slower than adding a text texture and using that

		Renders text without having to create a texture first

		@param[in] fontID ID of font to use
		@param[in] text to render
		@param[in] dest_rect what area to render into
		@param[in] color of text to render
		@param[in] b blend mode of texture 

		@return success

		@exception could not make text texture, returns false
		@exception could not render texture, returns false
		@exception could not free texture, returns false
	*/
	ENGINE_API bool renderText(const std::string& fontID, const std::string& text, const rect2<int32>& dest_rect, const v4<uint8>& color = v4<uint8>(255,255,255,255), blendmode b = blend_alpha);

private:
	logMgr logger;

	std::map<std::string,std::unique_ptr<texture>> textures;
	std::map<std::string,std::unique_ptr<font>> fonts;

	void* sdl_renderer;
	void* sdl_window;
	bool good;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
