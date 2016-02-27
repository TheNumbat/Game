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

timeMgr::timeMgr()
{
	good = false;
	logger.StartLog("TIME");
}

timeMgr::~timeMgr()
{
	timers.clear();
	kill();
}

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
	entry->second->pause = getTimeSinceStart() - entry->second->start - entry->second->lag;
	#ifdef VERBOSE_TIME
		logger.LogInfo("Paused timer ID " + ID);
	#endif
	return true;
}

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
	entry->second->lag = getTimeSinceStart() - entry->second->pause - entry->second->start;
	entry->second->pause = 0;
	#ifdef VERBOSE_TIME
		logger.LogInfo("Resumed timer ID " + ID);
	#endif
	return true;
}

uint64 timeMgr::get(const std::string& ID)
{
	// Get timer
	auto entry = timers.find(ID);
	if(entry == timers.end())
	{
		// Failure
		logger.LogWarn("Timer / perf counter to get ID " + ID + " not found");
		return 0;
	}

	// Test paused
	if(entry->second->pause)
	{
		// Success
		return entry->second->pause;
	}

	// Success
	return getTimeSinceStart() - entry->second->start - entry->second->lag;
}

uint64 timeMgr::getPerfFreq()
{
	return SDL_GetPerformanceFrequency();
}

uint32 timeMgr::getTimeSinceStart()
{
	return SDL_GetTicks();
}

// Terminating precompiler directives  ////////////////////////////////////////
