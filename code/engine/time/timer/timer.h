// Program Information ////////////////////////////////////////////////////////

/**
	@file timer.h

	@brief Declartion file for timer class

	Timer stores information for a single timer or performance counter

	@author Max Slater

	@version 1.00 (13 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include <memory>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Holds data describing a timer or performance counter

	Used by timeMgr.
*/
class timer
{
	timer(uint64 s, uint64 p);
	~timer();

	uint64 start;
	uint64 pause;
	uint64 lag;

	friend class timeMgr;

	// For memory
	friend class std::default_delete<timer>;
	friend std::unique_ptr<timer> std::make_unique<timer,uint64&,uint64>(uint64&,uint64&&);
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
