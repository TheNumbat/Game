// Program Information ////////////////////////////////////////////////////////

/**
	@file soundMgr.h

	@brief Declartion file for soundMgr class

	Manges the outout of sound, simalarly to how graphicMgr manages the output
	of textures.

	@author Max Slater

	@version 1.00 (10 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include "log/logMgr.h"
#include "sound/sound.h"

#include <string>
#include <memory>
#include <map>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Manges the outout of sound, simalarly to how graphicMgr manages the output
	of textures.
*/
class soundMgr
{
public:
	ENGINE_API soundMgr();
	ENGINE_API ~soundMgr();

	ENGINE_API bool init();
	ENGINE_API bool kill();

	ENGINE_API bool loadSound(const std::string& path, const std::string& ID = "");
	ENGINE_API bool loadSoundRec(const std::string& path);
	ENGINE_API bool freeSound(const std::string& ID);

	ENGINE_API bool pauseAll();
	ENGINE_API bool stopAll(int timeout = 0);
	ENGINE_API bool resumeAll();

	ENGINE_API bool pause(const std::string& ID);
	ENGINE_API bool play(const std::string& ID, int loop = 0, int timeout = -1);
	ENGINE_API bool resume(const std::string& ID);
	ENGINE_API bool stop(const std::string& ID, int timeout = 0);

private:
	logMgr logger;

	std::map<std::string,std::unique_ptr<sound>> sounds;
	
	bool good;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
