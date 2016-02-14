// Program Information ////////////////////////////////////////////////////////

/**
	@file timeMgr.cpp

	@brief Implementation file for timeMgr class

	Mangages timing and callbacks

	@author Max Slater

	@version 1.00 (13 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "timeMgr.h"

#include <SDL.h>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

/**
	@brief timeMgr constructor

	Defaults values and starts log
	Call init() to set up time
*/
timeMgr::timeMgr()
{
	good = false;
	logger.StartLog("TIME");
}

/**
	@brief timeMgr destructor

	Kills timer system, removes all timers
	Calls kill()
*/
timeMgr::~timeMgr()
{
	kill();
}

/**
	@brief initializes time system

	Sets up SDL timer system, etc.
	Call kill() to uninitialize

	@return success

	@exception already initialized, returns false
	@execption could not initialize SDL timer subsystem, resturns false 
*/
bool timeMgr::init()
{
	if(good) 
	{
		logger.LogWarn("Cannot reinitialize time!");
		return false;
	}

	// Open audio
	int result = SDL_InitSubSystem(SDL_INIT_TIMER);
	assert(result == 0);
	if(result != 0) 
	{
		logger.LogFatal((std::string)"Failed to initalize timer. SDL_Error: " + SDL_GetError());
		return false;
	}
	
	// Success
	good = true;
	logger.LogInfo("Time initialized");
	return true;
}

/**
	@brief deinitializes time system

	Kills SDL timer system

	@return success

	@exception already deinitialized, returns false
*/
bool timeMgr::kill()
{
	if(!good)
	{
		logger.LogWarn("Cannot re-destroy time!");
		return false;
	}

	// Success
	SDL_QuitSubSystem(SDL_INIT_TIMER);
	good = false;
	logger.LogInfo("Time uninitialized");
	return true;
}

/**
	@brief adds a timer
	
	@param[in] ID of new timer
	@param[in] startPaused wether or not to start the timer paused

	@return success

	@exception ID already taken, returns false
*/
bool timeMgr::addTimer(const std::string& ID, bool startPaused)
{
	// Test ID
	if(timers.find(ID) != timers.end())
	{
		logger.LogWarn("Timer ID: " + ID + " already in use!");
		return false;
	}

	// Create timer
	uint64 currentTime = getTimeSinceStart();
	std::unique_ptr<timer> newT = std::make_unique<timer>(currentTime,startPaused ? currentTime : 0);
	
	// Success
	timers.insert({ID,std::move(newT)});
	return true;
}

/**
	@brief adds a callback function to be called after a certain amount of time
	
	Callback functions must have the signature of uint32 callback(uint32 interval, void* parameter)
	where interval is "delay" in this function, and the return is the number of millisecons to wait until
	calling the callback funciton again, or 0 to not call it again.

	The callback function may be run in a new thread, so to call back into the normal
	thread one should use SDL_PushEvent within the callback and actually process the callback
	in the event loop

	Currently, these cannot be canceled, so be sure your function will still be
	available/valid when it is called.

	@param[in] callback pointer to callback function
	@param[in] delay to wait before calling the callback (in millisecons)
	@param[in] param to give to the callback function when it is called

	@return success

	@exception failed to create callback, returns false
*/
bool timeMgr::addCallback(uint32 (*callback)(uint32,void*), uint32 delay, void* param)
{
	SDL_TimerID result = SDL_AddTimer(delay,(SDL_TimerCallback)callback,param);
	assert(result != 0);
	if(result == 0)
	{
		logger.LogWarn((std::string)"Could not create callback , SDL_GetError: " + SDL_GetError());
		return false;
	}
	return true;
}

bool timeMgr::addPerfCounter(const std::string& ID)
{
	return false;
}

bool timeMgr::remove(const std::string& ID)
{
	return false;
}

bool timeMgr::pause(const std::string& ID)
{
	return false;
}

bool timeMgr::resume(const std::string& ID)
{
	return false;
}

bool timeMgr::get(const std::string& ID, uint64& time)
{
	return false;
}

uint64 timeMgr::getPerfFreq()
{
	return 0;
}

uint32 timeMgr::getTimeSinceStart()
{
	return 0;
}

// Terminating precompiler directives  ////////////////////////////////////////
