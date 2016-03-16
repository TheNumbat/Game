// Program Information ////////////////////////////////////////////////////////

/**
	@file threadMgr.h

	@brief Declartion file for threadMgr class

	Manges threading for the game.

	@author Max Slater

	@version 1.00 (12 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include "log/logMgr.h"

#include <string>
#include <map>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Manges threading for the game.

	Can add, wait, and detatch threads

	@note MAKE SURE YOUR THREAD WILL QUIT BEFORE WAITING ON IT OR ELSE
*/
class threadMgr 
{
public:
	/**
		@brief threadMgr constructur

		Starts log
	*/
	ENGINE_API threadMgr();

	/**
		@brief threadMgr deconstructur

		Make sure your threads will exit before destroying the manager!!!
	*/
	ENGINE_API ~threadMgr();

	/**
		@brief adds a thread

		Make sure your threads will exit before waiting on them!!!

		@param[in] ID of thread to add
		@param[in] threadFunction function to turn into a thread
		@param[in] data to pass to thread, MAKE SURE THIS INCLUDES A WAY TO END THE THREAD

		@return success

		@exception could not create thread -- returns false
	*/
	ENGINE_API bool add(const std::string& ID, int (*threadFunction)(void*), void* data);

	/**
		@brief waits on and removes a thread

		Make sure your threads will exit before waiting on them!!!

		@param[in] ID of thread to wait
		@param[out] returned value from thread. Default NULL to not capture return.

		@return success 

		@exception could not find thread -- returns false
	*/
	ENGINE_API bool wait(const std::string& ID, int* returned = NULL);

	/**
		@brief detaches a thread, relies on it to end itself

		The manager will no longer have access to it, so be sure it will behave.

		@param[in] ID of thread to detach

		@return success 

		@exception could not find thread -- returns false
	*/
	ENGINE_API bool detach(const std::string& ID);

	/**
		@brief waits on all threads

		@return success -- always true

		@note see wait()
	*/
	ENGINE_API bool waitAll();

	/**
		@brief detaches all threads

		@return success -- always true

		@note see detach()
	*/
	ENGINE_API bool detachAll();

private:
	/// Logger
	logMgr logger;
	/// Map of current (accesible) threads
	std::map<std::string,void*> threads;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
