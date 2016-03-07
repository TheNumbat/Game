// Program Information ////////////////////////////////////////////////////////

/**
	@file debugMgr.h

	@brief Declartion file for the debugMgr class.

	See debugMgr.cpp for member function documentation

	@author Max Slater

	@version 1.00 (6 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "game_common.h"

#include <log/logMgr.h>
#include <memory>
#include <map>

// Global constant definitions  ///////////////////////////////////////////////

class engine_state;

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief manages input for the game: depending on the input state events will
		   do different things
*/
class debugMgr
{
public:
	/**
		@brief debugMgr constructor

		Sets up log and such

		@param[in] e pointer to engine state
	*/
	debugMgr(engine_state* e);

	/**
		@brief debugMgr constructor

		Does nothing;
	*/
	~debugMgr();

	/**
		@brief Starts debug information for the frame
	*/
	void beginDebugFrame();

	/**
		@brief Ends debug information for the frame

		Updates average, lastFrameTime, clears the profiler if not paused
	*/
	void endDebugFrame();

	/**
		@brief Starts the profiling of a function

		Will be included in the profiler tree
	*/
	void beginProfiledFunc(const std::string& name);
	#define beginProfiledFunc() beginProfiledFunc(__func__);
	
	/**
		@brief Ends the profiling of a function

		Will be included in the profiler tree
	*/
	void endProfiledFunc();

	/**
		@brief Resets the average frame timer
	*/
	void resetAvgFrame();

	/**
		@brief Gets the average frame time

		@return average frame time in performance units
	*/
	uint64 getAvgFrame();

	/**
		@brief Gets the last frame time

		@return last frame time in performance units
	*/
	uint64 getLastFrame();

	/**
		@brief Sets the fps to cap to (will just stay in a while loop)

		@param[in] fps new fps cap or 0 for none

		@todo do something useful like simulate instead of a while loop
	*/
	void setFPSCap(uint8 fps = 0);

	/**
		@brief Gets the current fps cap

		@return current fps cap
	*/
	uint8 getFPSCap();

	/**
		@brief Pauses/resumes the profiler for functions
	*/
	void toggleProfiler();

private:
	class profileNode
	{
	public:
		/**
			@brief profileNode constructor

			Creates a new profiler node for a function
		*/
		profileNode(const std::string& func, uint64 s, std::weak_ptr<profileNode> parent_ = std::weak_ptr<profileNode>());
	private:
		std::string funcName;
		uint64 start;
		uint64 self;
		uint64 heir;
		uint32 calls;
		std::map<std::string,std::shared_ptr<profileNode>> children;
		std::weak_ptr<profileNode> parent;
		friend class debugMgr;
		friend class renderMgr;
	};

	bool paused;
	bool toggleAtEnd;

	uint64 totalFrameTime;
	uint64 totalFrames;

	uint64 lastFrameTime;

	uint8 fpsCap;

	std::shared_ptr<profileNode> profileHead;
	std::weak_ptr<profileNode> currentNode;
	engine_state* engine;

	logMgr logger;
	friend class renderMgr;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
