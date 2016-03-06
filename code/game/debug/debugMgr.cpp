// Program Information ////////////////////////////////////////////////////////

/**
	@file debugMgr.cpp

	@brief Implementation file for the debugMgr class.

	@author Max Slater

	@version 1.00 (6 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "debugMgr.h"

#include <engine_state.h>

#undef beginProfiledFunc()

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

debugMgr::debugMgr(engine_state* e)
{
	engine = e;
	engine->time.addPerfCounter("debug");
	lastFrameTime = 0;
	totalFrameTime = 0;
	totalFrames = 0;
	fpsCap = 0;
}

debugMgr::~debugMgr()
{
	engine->time.remove("debug");
}

debugMgr::profileNode::profileNode(const std::string& func, uint64 sStart, uint64 hStart)
{
	funcName = func;
	self = sStart;
	heir = hStart;
	calls = 1;
}

void debugMgr::beginDebugFrame()
{
	engine->time.reset("debug");
}

void debugMgr::endDebugFrame()
{
	if(fpsCap)
	{
		while(engine->time.get("debug") < engine->time.getPerfFreq() / fpsCap);
	}
	uint64 frame = engine->time.get("debug");
	lastFrameTime = frame;
	totalFrameTime += frame;
	totalFrames++;
}

void debugMgr::beginProfiledFunc(const std::string& name)
{

}

void debugMgr::endProfiledFunc()
{

}

void debugMgr::resetAvgFrame()
{
	totalFrameTime = 0;
	totalFrames = 0;
}

uint64 debugMgr::getAvgFrame()
{
	if(totalFrames)
	{
		return totalFrameTime / totalFrames;
	}
	return 0;
}

uint64 debugMgr::getLastFrame()
{
	return lastFrameTime;
}

void debugMgr::setFPSCap(uint8 fps)
{
	fpsCap = fps;
}

uint8 debugMgr::getFPSCap()
{
	return fpsCap;
}

// Terminating precompiler directives  ////////////////////////////////////////
