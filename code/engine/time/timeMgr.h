// Program Information ////////////////////////////////////////////////////////

/**
	@file timeMgr.h

	@brief Declartion file for timeMgr class

	Mangages timing and callbacks

	@author Max Slater

	@version 1.00 (13 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include "log/logMgr.h"
#include "timer/timer.h"

#include <string>
#include <memory>
#include <map>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Manges timing, performance counting, and callbacks
*/
class timeMgr
{
public:
	/**
		@brief timeMgr constructor

		Defaults values and starts log
		Call init() to set up time
	*/
	ENGINE_API timeMgr();

	/**
		@brief timeMgr destructor

		Kills timer system, removes all timers
		Calls kill()
	*/
	ENGINE_API ~timeMgr();

	/**
		@brief initializes time system

		Sets up SDL timer system, etc.
		Call kill() to uninitialize

		@return success

		@exception already initialized, returns false
		@exception could not initialize SDL timer subsystem, resturns false 
	*/
	ENGINE_API bool init();

	/**
		@brief deinitializes time system

		Kills SDL timer system

		@return success

		@exception already deinitialized, returns false
	*/
	ENGINE_API bool kill();

	/**
		@brief adds a timer
		
		@param[in] ID of new timer
		@param[in] startPaused wether or not to start the timer paused

		@return success

		@exception ID already taken, returns false
	*/
	ENGINE_API bool addTimer(const std::string& ID, bool startPaused = false);

	/**
		@brief adds a performance counter, which is basically a much higher-fidelity timer

		Call getPerfFreq() for the number of increments per second, as it is platform
		dependant.

		@param[in] ID for new performance counter
		@param[in] startPaused whether or not to start the perf counter paused

		@return success

		@exception ID already in use, returns false
	*/
	ENGINE_API bool addPerfCounter(const std::string& ID, bool startPaused = false);

	/**
		@brief removes a timer or performance counter

		@param[in] ID of timer/perfCounter to remove

		@return success

		@exception ID not found, returns false
	*/
	ENGINE_API bool remove(const std::string& ID);

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
	ENGINE_API bool addCallback(uint32 (*callback)(uint32,void*), uint32 delay, void* param, bool save = false, const std::string& ID = "");

	/**
		@brief removes a callback function

		The callback may have already ended, but it will still be stored in the callback table,
		so remove your callbacks when you're done with them.

		@param[in] ID of callback to remove

		@return success

		@exception ID not found, returns false
	*/
	ENGINE_API bool removeCallback(const std::string& ID);

	/**
		@brief Pauses a timer or performance counter

		@param[in] ID of timer/perfCounter to pause

		@return success

		@exception ID not found, returns false
		@exception timer arleady paused, returns false
	*/
	ENGINE_API bool pause(const std::string& ID);

	/**
		@brief Resumes a timer or performance counter

		@param[in] ID of timer/perfCounter to resume

		@return success

		@exception ID not found, returns false
		@exception timer arleady playing, returns false
	*/
	ENGINE_API bool resume(const std::string& ID);

	/**
		@brief Resets a timer or performance counter

		@param[in] ID of timer/perfCounter to reset

		@return success

		@exception ID not found, returns false
	*/
	ENGINE_API bool reset(const std::string& ID);

	/**
		@brief Gets the value from a timer or performance counter

		@param[in] ID of timer/perfCounter to retrieve time from

		@return time value gotten from timer/perf counter

		@exception ID not found, returns 0
	*/
	ENGINE_API uint64 get(const std::string& ID);

	/**
		@brief Gets the system-specific performance counter frequency

		Returns number of increments per second

		@return perf counter requency
	*/
	ENGINE_API uint64 getPerfFreq();

	/**
		@brief Gets the time in milliseconds since SDL was initialized

		@return time in milliseconds since SDL was initialized
	*/
	ENGINE_API uint32 getTimeSinceStart();

	/**
		@brief Gets the performance counter since SDL was initialized

		@return performance counters ince SDL was initialized
	*/
	ENGINE_API uint64 getPerfSinceStart();

private:
	logMgr logger;

	std::map<std::string,std::unique_ptr<timer>> timers;
	std::map<std::string,uint32> callbacks;
	
	bool good;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
