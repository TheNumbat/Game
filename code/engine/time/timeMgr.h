// Program Information ////////////////////////////////////////////////////////

/**
	@file timeMgr.h

	@brief Declartion file for timeMgr class

	Mangages timing and callbacks

	@author Max Slater

	@version 1.00 (13 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include "log/logMgr.h"
#include "timer/timer.h"

#include <string>
#include <memory>
#include <map>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Manges the outout of time, simalarly to how graphicMgr manages the output
	of textures.
*/
class timeMgr
{
public:
	ENGINE_API timeMgr();
	ENGINE_API ~timeMgr();

	ENGINE_API bool init();
	ENGINE_API bool kill();

	ENGINE_API bool addTimer(const std::string& ID, bool startPaused = false);
	ENGINE_API bool addCallback(uint32 (*callback)(uint32,void*), uint32 delay, void* param);
	ENGINE_API bool addPerfCounter(const std::string& ID);
	ENGINE_API bool remove(const std::string& ID);

	ENGINE_API bool pause(const std::string& ID);
	ENGINE_API bool resume(const std::string& ID);
	ENGINE_API bool get(const std::string& ID, uint64& time);

	ENGINE_API uint64 getPerfFreq();
	ENGINE_API uint32 getTimeSinceStart();

private:
	logMgr logger;

	std::map<std::string,std::unique_ptr<timer>> timers;
	
	bool good;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
