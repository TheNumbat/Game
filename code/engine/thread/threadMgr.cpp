// Program Information ////////////////////////////////////////////////////////

/**
	@file soundMgr.cpp

	@brief Implementaiton file for soundMgr class

	Manges the outout of sound, simalarly to how graphicMgr manages the output
	of textures.

	@author Max Slater

	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "threadMgr.h"

#include <SDL_thread.h>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

threadMgr::threadMgr()
{
	logger.StartLog("THREAD");
	logger.LogInfo("Threading initialized");
}

threadMgr::~threadMgr()
{
	waitAll();
	logger.LogInfo("Threading deinitialized");
}

bool threadMgr::add(const std::string& ID, int (*threadFunction)(void*), void* data)
{
	SDL_Thread* newT = SDL_CreateThread(threadFunction, ID.c_str(), data);
	if(!newT) 
	{
		// Failure
		logger.LogWarn("Could not create thread ID " + ID + ", SDL_Error: " + SDL_GetError());
		return false;
	}

	// Success
	threads.insert({ID,newT});
	#ifdef VERBOSE_THREAD
		logger.LogInfo("Added thread ID " + ID);
	#endif
	return true;
}

bool threadMgr::wait(const std::string& ID, int* returned)
{
	// Get thread
	auto entry = threads.find(ID);
	if(entry == threads.end())
	{
		// Failure
		logger.LogWarn("Could not find thread to wait on, ID " + ID);
		return false;
	}

	// Wait thread
	#ifdef VERBOSE_THREAD
		logger.LogInfo("Waiting on thread ID " + ID);	
	#endif
	SDL_WaitThread((SDL_Thread*)entry->second, returned);

	// Success
	threads.erase(entry);
	return true;
}

bool threadMgr::detach(const std::string& ID)
{
	// Get thread
	auto entry = threads.find(ID);
	if(entry == threads.end())
	{
		// Failure
		logger.LogWarn("Could not find thread to detach, ID " + ID);
		return false;
	}

	// Detach thread
	SDL_DetachThread((SDL_Thread*)entry->second);
	#ifdef VERBOSE_THREAD
		logger.LogInfo("Detached thread ID " + ID);	
	#endif

	// Success
	threads.erase(entry);
	return true;
}

bool threadMgr::waitAll()
{
	#ifdef VERBOSE_THREAD
		logger.LogInfo("Waiting on all threads");
		logger.EnterSec();
	#endif

	while(threads.size())
	{
		wait(threads.begin()->first);
	}

	#ifdef VERBOSE_THREAD
		logger.ExitSec();
	#endif

	return true;
}

bool threadMgr::detachAll()
{
	#ifdef VERBOSE_THREAD
		logger.LogInfo("Detaching all threads");
		logger.EnterSec();
	#endif

	while(threads.size())
	{
		detach(threads.begin()->first);
	}

	#ifdef VERBOSE_THREAD
		logger.ExitSec();
	#endif

	return true;
}

// Terminating precompiler directives  ////////////////////////////////////////
