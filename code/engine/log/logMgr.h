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

/// Use default level specified at creation
const int32 DEFAULT = 0;
/// Append INFO to header
const int32 INFO = 1;
/// Append WARNING to header
const int32 WARNING = 2;
/// Append FATAL to header
const int32 FATAL = 3;

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
	/**
		@brief Constructor for logMgr

		Sets initlized to false.
		
		@note Must call StartLog() to actually begin logging.
	*/
	ENGINE_API logMgr();

	/**
		@brief Destructor for logMgr

		cl2 logger will destruct itself.
	*/
	ENGINE_API ~logMgr();

	/**
		@brief Sets up and starts the logging system

		@param[in] defaultLvl is the text that will be used when logging
				   default level messages. Should pertain to who is doing
				   the logging.
		@param[in] header defaulted to false, marks wether or not to display
				   the header and time stamp when the log is started.
	*/
	ENGINE_API void StartLog(const std::string& defaultLvl, bool header = false);

	/**
		@brief Logs a messages at specified alert level.

		@param[in] msg is the message string to log
		@param[in] lvl is the alert level 

		@see alert levels in logMgr.h
	*/
	ENGINE_API void LogMsg(const std::string& msg, int lvl);


	/**
		@brief Logs a raw message

		@param[in] msg is the raw message string to log

		@see Ask Matt Bauer about the formatting, or see cyanlogger2.h
	*/
	ENGINE_API void LogRaw(const std::string& msg);

	/**
		@brief Logs a messages direclty to cout -- use for debugging purposes

		@param[in] msg is the raw message string to log
	*/
	ENGINE_API void LogDebug(const std::string& msg);

	/**
		@brief Logs a messages at the default level

		Default level specifed by StartLog()

		@param[in] msg is the message string to log

		@note Calls LogMsg()
	*/
	ENGINE_API void LogDefault(const std::string& msg);

	/**
		@brief Logs a messages at the info level

		Info level is deafult level + /INFO

		@param[in] msg is the message string to log

		@note Calls LogMsg()
	*/
	ENGINE_API void LogInfo(const std::string& msg);

	/**
		@brief Logs a messages at the warning level

		Warning level is deafult level + /WARNING

		@param[in] msg is the message string to log

		@note Calls LogMsg()
	*/
	ENGINE_API void LogWarn(const std::string& msg);

	/**
		@brief Logs a messages at the fatal level

		Fatal level is deafult level + /FATAL

		@param[in] msg is the message string to log

		@note Calls LogMsg()
	*/
	ENGINE_API void LogFatal(const std::string& msg);

	/**
		@brief Enters an indentation section for logging

		Must call ExitSec() to leave the section

		Always remember to exit the sections you enter.
	*/
	ENGINE_API void EnterSec();

	/**
		@brief Exits an indentation section for logging

		Must have first entered an indentation section or won't do anything.

		Always remember to exit the sections you enter.
	*/
	ENGINE_API void ExitSec();

private:
	/// Matt bauer's logger
	cl2::Log logger;
	/// Default level text
	std::string defaultLevel;
	/// Logger is good
	bool good;
};

/// Add file and line number to warning messages
#define LogWarn(a) LogWarn(((std::string)__FILE__).substr(((std::string)__FILE__).find_last_of("\\/") + 1,((std::string)__FILE__).find_last_of("\\/") - ((std::string)__FILE__).length()) + ", in " + (std::string)__func__ + "(), line " + std::to_string(__LINE__) + ": " + (a))
/// Add file and line number to error messages
#define LogFatal(a) LogFatal(((std::string)__FILE__).substr(((std::string)__FILE__).find_last_of("\\/") + 1,((std::string)__FILE__).find_last_of("\\/") - ((std::string)__FILE__).length()) + ", in " + (std::string)__func__ + "(), line " + std::to_string(__LINE__) + ": " + (a))

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
