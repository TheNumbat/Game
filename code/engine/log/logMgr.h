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
#include <cyanlogger2.h>

// Global constant definitions  ///////////////////////////////////////////////

const int DEFAULT = 0;
const int INFO = 1;
const int WARNING = 2;
const int FATAL = 3;

// Class/Struct definitions  //////////////////////////////////////////////////

class logMgr
{
public:
	logMgr();
	~logMgr();

	void StartLog(const std::string& defaultLvl, bool header = false);

	void LogMsg(const std::string& msg, int lvl);
	void LogRaw(const std::string& msg);

	void LogDefault(const std::string& msg);
	void LogInfo(const std::string& msg);
	void LogWarn(const std::string& msg);
	void LogFatal(const std::string& msg);

	void EnterSec();
	void ExitSec();

private:
	cl2::Log logger;
	bool init;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
