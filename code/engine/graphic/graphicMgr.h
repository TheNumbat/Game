// Program Information ////////////////////////////////////////////////////////

/**
	@file graphicMgr.h

	@brief Implementaiton file for graphicMgr class

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
#include "camera\camera.h"
#include "log\logMgr.h"

#include "rect.h"

#include <map>
#include <string>
#include <memory>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Manages graphical display

	See function docuemtnation
*/
class graphicMgr
{
public:
	ENGINE_API graphicMgr();
	ENGINE_API ~graphicMgr();

	ENGINE_API bool init(const std::string& winTitle, uint32 sw = 1280, uint32 sh = 720);
	ENGINE_API bool kill();
	
	ENGINE_API bool displayFrame(bool clearAfter = true);

	ENGINE_API bool loadTexture(const std::string& path, const std::string& ID);
	ENGINE_API bool loadTextureRec(const std::string& path);
	ENGINE_API bool freeTexture(const std::string& ID);

	/// @todo support for rotation and source rect
	ENGINE_API bool renderTexture(const std::string& ID, const rect2<int32>& dest_rect);

private:
	logMgr logger;

	std::map<std::string,std::unique_ptr<texture>> textures;
	camera cam;

	void* sdl_renderer;
	void* sdl_window;
	bool good;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
