// Program Information ////////////////////////////////////////////////////////

/**
	@file timer.cpp

	@brief Implementation file for timer class

	Timer stores information for a single timer or performance counter

	@author Max Slater

	@version 1.00 (13 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "timer.h"

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

/**
	@breif Timer constructor

	Sets up values

	@param[in] s start time in time units (ms for normal timer)
	@param[in] p pause time if starting paused (ms for normal timer)
	@param[in] pC wheter or not the timer is a performance counter
*/
timer::timer(uint64 s, uint64 p, bool pC)
{
	start = s;
	pause = p;
	perfCounter = pC;
	lag = 0;
}

/**
	@breif Timer denstructor

	Does nothing
*/
timer::~timer()
{

}


// Terminating precompiler directives  ////////////////////////////////////////
