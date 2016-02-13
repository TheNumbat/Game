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
	ENGINE_API threadMgr();
	ENGINE_API ~threadMgr();

	ENGINE_API bool add(const std::string& ID, int (*threadFunction)(void*), void* data);
	ENGINE_API bool wait(const std::string& ID, int* returned = NULL);
	ENGINE_API bool detach(const std::string& ID);

	ENGINE_API bool waitAll();
	ENGINE_API bool detachAll();

private:
	logMgr logger;
	std::map<std::string,void*> threads;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
