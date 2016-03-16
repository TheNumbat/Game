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
	/**
		@brief Timer constructor

		Sets up values

		@param[in] s start time in time units (ms for normal timer)
		@param[in] p pause time if starting paused (ms for normal timer)
		@param[in] pC wheter or not the timer is a performance counter
	*/
	timer(uint64 s, uint64 p, bool pC);

	/**
		@brief Timer denstructor

		Does nothing
	*/
	~timer();

	/// Start time (ms or perf)
	uint64 start;
	/// Time when paused (ms or perf)
	uint64 pause;
	/// Lag time behind current time (ms or perf)
	uint64 lag;

	/// If this timer is a performance counter or not
	bool perfCounter;

	friend class timeMgr;

	/// Friendship for smart pointers
	friend class std::default_delete<timer>;
	/// Friendship for smart pointers
	friend std::unique_ptr<timer> std::make_unique<timer,uint64&,uint64,bool>(uint64&,uint64&&,bool&&);
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
