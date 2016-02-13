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

/**
	@brief threadMgr constructur

	Starts log
*/
threadMgr::threadMgr()
{
	logger.StartLog("THREAD");
}

/**
	@brief threadMgr deconstructur

	Make sure your threads will exit before destroying the manager!!!
*/
threadMgr::~threadMgr()
{
	waitAll();
}

/**
	@brief adds a thread

	Make sure your threads will exit before waiting on them!!!

	@param[in] ID of thread to add
	@param[in] threadFunction function to turn into a thread
	@param[in] data to pass to thread, MAKE SURE THIS INCLUDES A WAY TO END THE THREAD

	@return success

	@exception could not create thread -- returns false
*/
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
	logger.LogInfo("Added thread ID " + ID);
	return true;
}

/**
	@brief waits on and removes a thread

	Make sure your threads will exit before waiting on them!!!

	@param[in] ID of thread to wait
	@param[out] returned value from thread. Default NULL to not capture return.

	@return success 

	@exception could not find thread -- returns false
*/
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
	logger.LogInfo("Waiting on thread ID " + ID);	
	SDL_WaitThread((SDL_Thread*)entry->second, returned);

	// Success
	threads.erase(entry);
	return true;
}

/**
	@brief detaches a thread, relies on it to end itself

	The manager will no longer have access to it, so be sure it will behave.

	@param[in] ID of thread to detach

	@return success 

	@exception could not find thread -- returns false
*/
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
	logger.LogInfo("Detached thread ID " + ID);	

	// Success
	threads.erase(entry);
	return true;
}

/**
	@brief waits on all threads

	@return success -- always true

	@note see wait()
*/
bool threadMgr::waitAll()
{
	while(threads.size())
	{
		wait(threads.begin()->first);
	}

	return true;
}

/**
	@brief detaches all threads

	@return success -- always true

	@note see detach()
*/
bool threadMgr::detachAll()
{
	while(threads.size())
	{
		detach(threads.begin()->first);
	}

	return true;
}

// Terminating precompiler directives  ////////////////////////////////////////
