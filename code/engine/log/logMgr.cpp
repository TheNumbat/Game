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

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

/**
	@brief Constructor for logMgr

	Sets initlized to false.
	
	@note Must call StartLog() to actually begin logging.
*/
logMgr::logMgr() {}

/**
	@brief Destructor for logMgr

	cl2 logger will destruct itself.
*/
logMgr::~logMgr() {}

/**
	@brief Sets up and starts the logging system

	@param[in] defaultLvl is the text that will be used when logging
			   default level messgaes. Should pertain to who is doing
			   the logging.
	@param[in] header defaulted to false, marks wether or not to display
			   the header and time stamp when the log is started.
*/
void logMgr::StartLog(const std::string& defaultLvl, bool header)
{
	if( !init )
	{
		// Destinations based on debug_defines.h
		#ifdef CONSOLE_LOG
			logger.AddDest(cl2::fh_stdout());
		#endif

		#ifdef FILE_LOG
			logger.AddDest("log.txt");
		#endif

		// Set up logger
		logger.SetMaxBuffSize(0);
		logger.AddLvlEntry(DEFAULT, "[" + defaultLvl + "] ");
		logger.AddLvlEntry(INFO, "[" + defaultLvl + "/INFO] ");
		logger.AddLvlEntry(WARNING, "[" + defaultLvl + "/WARNING] ");
		logger.AddLvlEntry(FATAL, "[" + defaultLvl + "/FATAL] ");
		logger.EnableInput();

		if( header )
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

		init = true;
	}
}

/**
	@brief Logs a messages at specified alert level.

	@param[in] msg is the message string to log
	@param[in] lvl is the alert level 

	@see alert levels in logMgr.h
*/
void logMgr::LogMsg(const std::string& msg, int lvl)
{
	logger.PutMsg(msg, lvl);
}

/**
	@brief Logs a raw message

	@param[in] msg is the raw message string to log

	@see Ask Matt Bauer about the formatting, or see cyanlogger2.h
*/
void logMgr::LogRaw(const std::string& msg)
{
	logger.PutRawMsg(msg);
}

/**
	@brief Logs a messages at the default level

	Default level specifed by StartLog()

	@param[in] msg is the message string to log

	@note Calls LogMsg
*/
void logMgr::LogDefault(const std::string& msg)
{
	LogMsg(msg, DEFAULT);
}

/**
	@brief Logs a messages at the info level

	Info level is deafult level + /INFO

	@param[in] msg is the message string to log

	@note Calls LogMsg
*/
void logMgr::LogInfo(const std::string& msg)
{
	LogMsg(msg, INFO);
}

/**
	@brief Logs a messages at the warning level

	Warning level is deafult level + /WARNING

	@param[in] msg is the message string to log

	@note Calls LogMsg
*/
void logMgr::LogWarn(const std::string& msg)
{
	LogMsg(msg, WARNING);
}

/**
	@brief Logs a messages at the fatal level

	Fatal level is deafult level + /FATAL

	@param[in] msg is the message string to log

	@note Calls LogMsg
*/
void logMgr::LogFatal(const std::string& msg)
{
	LogMsg(msg, FATAL);
}

/**
	@brief Enters an indentation section for logging

	@note Must call ExitSec() to leave the section
*/
void logMgr::EnterSec()
{
	logger.EnterSec();
}

/**
	@brief Exits an indentation section for logging
*/
void logMgr::ExitSec()
{
	logger.ExitSec();
}

// Terminating precompiler directives  ////////////////////////////////////////
