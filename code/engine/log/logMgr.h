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
	API logMgr();
	API ~logMgr();

	API void StartLog(const std::string& defaultLvl, bool header = false);

	API void LogMsg(const std::string& msg, int lvl);
	API void LogRaw(const std::string& msg);
	API void LogDebug(const std::string& msg);

	API void LogDefault(const std::string& msg);
	API void LogInfo(const std::string& msg);
	API void LogWarn(const std::string& msg);
	API void LogFatal(const std::string& msg);

	API void EnterSec();
	API void ExitSec();

private:
	cl2::Log logger;
	std::string defaultLevel;
	bool good;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
