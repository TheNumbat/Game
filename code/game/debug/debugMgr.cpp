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
	paused = false;
	toggleAtEnd = false;
}

debugMgr::~debugMgr()
{
	engine->time.remove("debug");
}

debugMgr::profileNode::profileNode(const std::string& func, uint64 start, std::weak_ptr<profileNode> parent_)
{
	funcName = func;
	self = start;
	heir = start;
	calls = 1;
	parent = parent_;
}

void debugMgr::toggleProfiler()
{
	toggleAtEnd = true;
}

void debugMgr::beginDebugFrame()
{
	engine->time.reset("debug");
}

void debugMgr::endDebugFrame()
{
	if(toggleAtEnd)
	{
		toggleAtEnd = false;
		paused = !paused;
	}

	if(fpsCap)
	{
		while(engine->time.get("debug") < engine->time.getPerfFreq() / fpsCap);
	}
	uint64 frame = engine->time.get("debug");
	lastFrameTime = frame;
	totalFrameTime += frame;
	totalFrames++;

	if(!paused)
	{
		profileHead.reset(); 
		currentNode = profileHead;
	}

	if(fpsCap && lastFrameTime > engine->time.getPerfFreq() / (fpsCap - 1))
	{
		engine->logger.LogWarn("Last frame took " + std::to_string(1000.0f * lastFrameTime / (real64)engine->time.getPerfFreq()) + " ms!");
	}
}

void debugMgr::beginProfiledFunc(const std::string& name)
{
	if(paused)
	{
		return;
	}

	uint64 current = engine->time.get("debug");

	if(currentNode.expired())
	{
		profileHead = std::make_shared<profileNode>(name,current);
		currentNode = profileHead;
		return;
	}

	auto entry = currentNode.lock()->children.find(name);
	if(entry == currentNode.lock()->children.end())
	{
		std::shared_ptr<profileNode> newN = std::make_shared<profileNode>(name,current,currentNode);
		currentNode.lock()->children.insert({name,newN});
		currentNode = newN;
		return;	
	}

	entry->second->calls++;
}

void debugMgr::endProfiledFunc()
{
	if(paused)
	{
		return;
	}

	uint64 current = engine->time.get("debug");

	currentNode.lock()->heir = current - currentNode.lock()->heir;

	uint64 exceptSelf = 0;

	for(auto entry : currentNode.lock()->children)
	{
		exceptSelf += entry.second->heir;
	}

	currentNode.lock()->self = currentNode.lock()->heir - exceptSelf;

	if(!currentNode.lock()->parent.expired())
	{
		currentNode = currentNode.lock()->parent;
	}
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
