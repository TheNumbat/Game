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
	std::unique_ptr<timer> newT = std::make_unique<timer>(currentTime,startPaused ? currentTime : 0,false);
	
	// Success
	timers.insert({ID,std::move(newT)});
	#ifdef VERBOSE_TIME
		logger.LogInfo("Added timer ID " + ID);
	#endif
	return true;
}

/**
	@brief adds a callback function to be called after a certain amount of time
	
	Callback functions must have the signature of uint32 callback(uint32 interval, void* parameter)
	where interval is "delay" in this function, and the return is the number of millisecons to wait until
	calling the callback funciton again, or 0 to not call it again. Make sure that the function
	will at some point return zero if you don't want it to infinitely run on an interval.

	The callback function may be run in a new thread, so to call back into the normal
	thread one should use SDL_PushEvent within the callback and actually process the callback
	in the event loop

	Finally, if your callback funtion will run forever and you need to be able to remove it, pass "true" to
	paramter save. If you leave it out or pass "false," your callback will not be saved in timeMgr and cannot
	be removed. Hence, if your callback will end itself don't save it, if you need to manually end it, save it.

	@param[in] callback pointer to callback function
	@param[in] delay to wait before calling the callback (in millisecons)
	@param[in] param to give to the callback function when it is called
	@param[in] save wheter or not to save callback in timeMgr (meaning it can be removed later)
	@param[in] ID for new callback -- only needed if saving the callback (save = true)

	@return success

	@exception ID already in use, returns false
	@exception failed to create callback, returns false
	@exception save = true but no ID, returns false does nothing
*/
bool timeMgr::addCallback(uint32 (*callback)(uint32,void*), uint32 delay, void* param, bool save, const std::string& ID)
{
	if(save && ID == "")
	{
		logger.LogWarn("Cannot create saved callback with no ID!");
		return false;
	}

	if(callbacks.find(ID) != callbacks.end())
	{
		logger.LogWarn("Callback ID " + ID + " already in use");
		return false;
	}

	SDL_TimerID result = SDL_AddTimer(delay,(SDL_TimerCallback)callback,param);
	assert(result != 0);
	if(result == 0)
	{
		logger.LogWarn((std::string)"Could not create callback , SDL_GetError: " + SDL_GetError());
		return false;
	}

	if(save)
	{
		callbacks.insert({ID,result});
	}
	#ifdef VERBOSE_TIME
		logger.LogInfo("Added callback ID " + ID);
	#endif
	return true;
}

/**
	@brief removes a callback function

	The callback may have already ended, but it will still be stored in the callback table,
	so remove your callbacks when you're done with them.

	@param[in] ID of callback to remove

	@return success

	@exception ID not found, returns false
*/
ENGINE_API bool timeMgr::removeCallback(const std::string& ID)
{
	auto entry = callbacks.find(ID);
	if(entry == callbacks.end())
	{
		logger.LogWarn("Saved callback ID " + ID + " not found");
		return false;
	}

	bool result = SDL_RemoveTimer(entry->second);
	callbacks.erase(entry);
	if(!result)
	{
		logger.LogWarn("Could not remove saved callback ID " + ID + " already ended.");
		return false;
	}
	#ifdef VERBOSE_TIME
		logger.LogInfo("Removed callback ID " + ID);
	#endif
	return true;
}

/**
	@brief adds a performance counter, which is basically a much higher-fidelity timer

	Call getPerfFreq() for the number of increments per second, as it is platform
	dependant.

	@param[in] ID for new performance counter

	@return success

	@exception ID already in use, returns false
*/
bool timeMgr::addPerfCounter(const std::string& ID, bool startPaused)
{
	// Test ID
	if(timers.find(ID) != timers.end())
	{
		// Failure
		logger.LogWarn("Perf counter ID: " + ID + " already in use!");
		return false;
	}

	// Create perf counter
	uint64 currentTime = getTimeSinceStart();
	std::unique_ptr<timer> newPC = std::make_unique<timer>(currentTime,startPaused ? currentTime : 0,true);
	
	// Success
	timers.insert({ID,std::move(newPC)});
	#ifdef VERBOSE_TIME
		logger.LogInfo("Added performace counter ID " + ID);
	#endif
	return true;
}

/**
	@brief removes a timer or performance counter

	@param[in] ID of timer/perfCounter to remove

	@return success

	@exception ID not found, returns false
*/
bool timeMgr::remove(const std::string& ID)
{
	// Get timer
	auto entry = timers.find(ID);
	if(entry == timers.end())
	{
		// Failure
		logger.LogWarn("Timer / perf counter ID " + ID + " not found");
		return false;
	}

	// Success
	timers.erase(entry);
	#ifdef VERBOSE_TIME
		logger.LogInfo("Removed timer/perf counter ID " + ID);
	#endif
	return true;
}

/**
	@brief Pauses a timer or performance counter

	@param[in] ID of timer/perfCounter to pause

	@return success

	@exception ID not found, returns false
	@exception timer arleady paused, returns false
*/
bool timeMgr::pause(const std::string& ID)
{
	// Get timer
	auto entry = timers.find(ID);
	if(entry == timers.end())
	{
		// Failure
		logger.LogWarn("Timer / perf counter to pause ID " + ID + " not found");
		return false;
	}

	// Test paused
	if(entry->second->pause)
	{
		// Failure
		logger.LogWarn("Timer ID " + ID + " already paused");
		return false;
	}

	// Success
	entry->second->pause = getTimeSinceStart();
	#ifdef VERBOSE_TIME
		logger.LogInfo("Paused timer ID " + ID);
	#endif
	return true;
}

/**
	@brief Resumes a timer or performance counter

	@param[in] ID of timer/perfCounter to resume

	@return success

	@exception ID not found, returns false
	@exception timer arleady playing, returns false
*/
bool timeMgr::resume(const std::string& ID)
{
	// Get timer
	auto entry = timers.find(ID);
	if(entry == timers.end())
	{
		// Failure
		logger.LogWarn("Timer / perf counter to resume ID " + ID + " not found");
		return false;
	}

	// Test not paused
	if(!entry->second->pause)
	{
		// Failure
		logger.LogWarn("Timer ID " + ID + " already playing");
		return false;
	}

	// Success
	entry->second->lag = getTimeSinceStart() - entry->second->pause;
	entry->second->pause = 0;
	#ifdef VERBOSE_TIME
		logger.LogInfo("Resumed timer ID " + ID);
	#endif
	return true;
}

/**
	@brief Gets the value from a timer or performance counter

	@param[in] ID of timer/perfCounter to retrieve time from

	@return success

	@exception ID not found, returns false
*/
bool timeMgr::get(const std::string& ID, uint64& time)
{
	// Get timer
	auto entry = timers.find(ID);
	if(entry == timers.end())
	{
		// Failure
		logger.LogWarn("Timer / perf counter to get ID " + ID + " not found");
		return false;
	}

	// Test paused
	if(entry->second->pause)
	{
		// Success
		time = entry->second->pause;
		return true;
	}

	// Success
	time = getTimeSinceStart() - entry->second->start - entry->second->lag;
	return true;
}

/**
	@brief Gets the system-specific performance counter frequency

	Returns number of increments per second

	@return perf counter requency
*/
uint64 timeMgr::getPerfFreq()
{
	return SDL_GetPerformanceFrequency();
}

/**
	@brief Gets the time in milliseconds since SDL was initialized

	@return time in milliseconds since SDL was initialized
*/
uint32 timeMgr::getTimeSinceStart()
{
	return SDL_GetTicks();
}

// Terminating precompiler directives  ////////////////////////////////////////
