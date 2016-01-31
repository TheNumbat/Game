// Program Information ////////////////////////////////////////////////////////

/**
	@file logMgr.h

	@brief Declartion file for logMgr class.

	logMgr does logging, it's pretty simple. Each other manager has it's own
	logMgr member to do logging.

	See logMgr.cpp for member function documentation

	@author Max Slater

	@version 1.00 (28 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include <string>
#include <iostream>
#include <cyanlogger2.h>

// Global constant definitions  ///////////////////////////////////////////////

const int DEFAULT = 0;
const int INFO = 1;
const int WARNING = 2;
const int FATAL = 3;

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Manges logging using Matt Bauer's logging system. 

	Used by all other managers for their logging.

	@note Matt's logging system is threaded, so do not depend on it for debugging
	purposes. To log straight to "cout" use LogDebug()
*/
class logMgr
{
public:
	ENGINE_API logMgr();
	ENGINE_API ~logMgr();

	ENGINE_API void StartLog(const std::string& defaultLvl, bool header = false);

	ENGINE_API void LogMsg(const std::string& msg, int lvl);
	ENGINE_API void LogRaw(const std::string& msg);
	ENGINE_API void LogDebug(const std::string& msg);

	ENGINE_API void LogDefault(const std::string& msg);
	ENGINE_API void LogInfo(const std::string& msg);
	ENGINE_API void LogWarn(const std::string& msg);
	ENGINE_API void LogFatal(const std::string& msg);

	ENGINE_API void EnterSec();
	ENGINE_API void ExitSec();

private:
	cl2::Log logger;
	std::string defaultLevel;
	bool good;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
