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

const uint32 FLIP_NONE = 0;
const uint32 FLIP_HORZ = 1<<0;
const uint32 FLIP_VERT = 1<<1;

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Manages graphical display

	See function docuemtnation

	@todo do we want this to render entities? Probably not, that would be too tightly 
		  coupled
*/
class graphicMgr
{
public:
	ENGINE_API graphicMgr();
	ENGINE_API ~graphicMgr();

	ENGINE_API bool init(const std::string& winTitle, uint32 sw = 1280, uint32 sh = 720);
	ENGINE_API bool kill();
	
	ENGINE_API bool displayFrame(bool clearAfter = true);

	ENGINE_API bool loadTexture(const std::string& path, const std::string& ID = "");
	ENGINE_API bool loadTextureRec(const std::string& path);
	ENGINE_API bool freeTexture(const std::string& ID);

	ENGINE_API bool renderTexture(const std::string& ID, const rect2<int32>& dest_rect);
	ENGINE_API bool renderTextureEx(const std::string& ID, const rect2<int32>& dest_rect,
									const rect2<int32>& src_rect, const v2<int32>& rot_point,
									real32 rot, uint32 flip);

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
