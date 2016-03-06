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

#include <memory>
#include <vector>

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

	void beginDebugFrame();
	void endDebugFrame();

	void beginProfiledFunc(const std::string& name);
	#define beginProfiledFunc() beginProfiledFunc(__func__);
	
	void endProfiledFunc();

	void resetAvgFrame();
	uint64 getAvgFrame();
	uint64 getLastFrame();

	void setFPSCap(uint8 fps = 0);
	uint8 getFPSCap();

private:
	class profileNode
	{
		profileNode(const std::string& func, uint64 sStart, uint64 hStart);
		std::string funcName;
		uint64 self;
		uint64 heir;
		uint32 calls;
		std::vector<std::shared_ptr<profileNode>> children;
		std::weak_ptr<profileNode> parent;
		friend class debugMgr;
	};

	uint64 totalFrameTime;
	uint64 totalFrames;

	uint64 lastFrameTime;

	uint8 fpsCap;

	std::shared_ptr<profileNode> currentNode;
	engine_state* engine;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
