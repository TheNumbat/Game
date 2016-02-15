// Program Information ////////////////////////////////////////////////////////

/**
	@file logMgr.cpp

	@brief Implementation file for logMgr class.

	logMgr does logging, it's pretty simple. Each other manager has it's own
	logMgr member to do logging.

	@author Max Slater
	
	@version 1.00 ()
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "logMgr.h"
#include <iostream>

#undef LogWarn
#undef LogFatal

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

logMgr::logMgr() 
{
	good = false;
}

logMgr::~logMgr() 
{

}

void logMgr::StartLog(const std::string& defaultLvl, bool header)
{
	if(!good)
	{
		defaultLevel = defaultLvl;

		// Destinations based on debug_defines.h
		#ifdef CONSOLE_LOG
			logger.AddDest(cl2::fh_stdout());
		#endif

		#ifdef FILE_LOG
			logger.AddDest("log.txt");
		#endif

		// Set up logger
		logger.SetMaxBuffSize(0);
		logger.EnableInput();
		logger.AddLvlEntry(DEFAULT, "[" + defaultLevel + "] ");
		logger.AddLvlEntry(INFO, "[" + defaultLevel + "/INFO] ");
		logger.AddLvlEntry(WARNING, "[" + defaultLevel + "/WARNING] ");
		logger.AddLvlEntry(FATAL, "[" + defaultLevel + "/FATAL] ");

		if(header)
		{
			// Print Log Header
			logger.SetMessageFormat("----- Game Design Log - %t -----\n\n");
			logger.SetTimestampFormat("%H:%m:%s %K %d %Y");
			logger.PutMsg("");
		}

		// Set up formatting
		logger.SetMessageFormat("%i%t %l%m\n");
		logger.SetRawMessageFormat("%i%m\n"); // NO ENDLINE
		logger.SetTimestampFormat("%H:%m:%s");

		good = true;
	}
	else
	{
		LogWarn("Attempting to reinitialize logging");
	}
}

void logMgr::LogMsg(const std::string& msg, int lvl)
{
	logger.PutMsg(msg, lvl);
}

void logMgr::LogRaw(const std::string& msg)
{
	logger.PutRawMsg(msg);
}

void logMgr::LogDebug(const std::string& msg)
{
	std::cout << "         [" << defaultLevel << "/DEBUG] "
			  << msg << std::endl;
}

void logMgr::LogDefault(const std::string& msg)
{
	LogMsg(msg, DEFAULT);
}

void logMgr::LogInfo(const std::string& msg)
{
	LogMsg(msg, INFO);
}

void logMgr::LogWarn(const std::string& msg)
{
	LogMsg(msg, WARNING);
}

void logMgr::LogFatal(const std::string& msg)
{
	LogMsg(msg, FATAL);
}

void logMgr::EnterSec()
{
	logger.EnterSec();
}

void logMgr::ExitSec()
{
	logger.ExitSec();
}

// Terminating precompiler directives  ////////////////////////////////////////
