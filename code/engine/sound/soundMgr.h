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
	ENGINE_API soundMgr();
	ENGINE_API ~soundMgr();

	ENGINE_API bool init();
	ENGINE_API bool kill();

	ENGINE_API bool loadSound(const std::string& path, int repeat_, const std::string& ID = "");
	ENGINE_API bool loadSoundRec(const std::string& path);
	ENGINE_API bool freeSound(const std::string& ID);

	ENGINE_API bool pauseAll();
	ENGINE_API bool playAll();
	ENGINE_API bool playpauseAll();
	ENGINE_API bool stopAll();

	ENGINE_API bool pause(const std::string& ID);
	ENGINE_API bool play(const std::string& ID);
	ENGINE_API bool playpause(const std::string& ID);
	ENGINE_API bool stop(const std::string& ID);

private:
	logMgr logger;

	std::map<std::string,std::unique_ptr<sound>> sounds;
	
	bool good;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
