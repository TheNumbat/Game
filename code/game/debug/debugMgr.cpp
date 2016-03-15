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

#include "game_state.h"
#include <engine_state.h>

#undef beginProfiledFunc()

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

debugMgr::debugMgr(game_state* g, engine_state* e)
{
	logger.StartLog("DEBUG");
	engine = e;
	game = g;
	engine->time.addPerfCounter("debug");
	
	lastFrameTime = 0;
	totalFrameTime = 0;
	totalFrames = 0;
	fpsCap = 0;
	debugFlags = 0;

	profileHead = std::make_shared<profileNode>("profiler",0);
	currentNode = profileHead;
	selected = profileHead;
	logger.LogInfo("Debug initialized");
}

debugMgr::~debugMgr()
{
	engine->time.remove("debug");
	logger.LogInfo("Debug deinitialized");
}

debugMgr::profileNode::profileNode(const std::string& func, uint64 s, std::weak_ptr<profileNode> parent_)
{
	funcName = func;
	showChildren = false;
	start = s;
	self = 0;
	heir = 0;
	calls = 1;
	parent = parent_;
}

void debugMgr::setDebugOption(uint64 option)
{
	debugFlags |= option;
}

void debugMgr::clearDebugOption(uint64 option)
{
	debugFlags &= ~option;
}

void debugMgr::toggleDebugOption(uint64 option)
{
	if(debugFlags & option)
	{
		clearDebugOption(option);
	}
	else
	{
		setDebugOption(option);
	}
}

bool debugMgr::callConsoleFunc(const std::string& input)
{
	std::string funcName = input.substr(0,input.find(' '));

	auto entry = consoleFuncs.find(funcName);
	if(entry == consoleFuncs.end())
	{
		void* func = engine->file.loadFunction("console",funcName);
		if(!func)
		{
			logger.LogWarn("Could not find or load function name " + funcName);
			return false;
		}

		consoleFuncs.insert({funcName,(consoleFunc)func});
		return (*(consoleFunc)func)(game,engine,input.substr(input.find(' ') + 1,input.size()));
	}
	else
	{
		return (*entry->second)(game,engine,input.substr(input.find(' ') + 1,input.size()));		
	}
}

void debugMgr::loadConsoleFuncs()
{
	consoleFuncs.clear();
	engine->file.freeLibrary("console");
	engine->file.copyFileBin("console/console.dll","console/consoleTemp.dll");
	engine->file.loadLibrary("console/consoleTemp.dll","console");
}

void debugMgr::beginDebugFrame()
{
	engine->time.reset("debug");
}

void debugMgr::endDebugFrame()
{
	if(debugFlags & toggleProfiler)
	{
		clearDebugOption(toggleProfiler);
		toggleDebugOption(profilerPaused);
	}

	if(fpsCap)
	{
		while(engine->time.get("debug") < engine->time.getPerfFreq() / fpsCap);
	}
	uint64 frame = engine->time.get("debug");
	lastFrameTime = frame;
	totalFrameTime += frame;
	totalFrames++;

	if(fpsCap && lastFrameTime > engine->time.getPerfFreq() / (fpsCap - 1))
	{
		engine->logger.LogWarn("Last frame took " + std::to_string(1000.0f * lastFrameTime / (real64)engine->time.getPerfFreq()) + " ms!");
	}

	if(!(debugFlags & profilerPaused))
	{
		currentNode = profileHead;
		resetNodesRecursive(currentNode);
	}
}

void debugMgr::resetNodesRecursive(std::weak_ptr<profileNode> current)
{
	for(auto& entry : current.lock()->children)
	{
		resetNodesRecursive(entry.second);
	}

	current.lock()->self = 0;
	current.lock()->heir = 0;
	current.lock()->calls = 0;
}

void debugMgr::beginProfiledFunc(const std::string& name)
{
	if(debugFlags & profilerPaused)
	{
		return;
	}

	uint64 current = engine->time.get("debug");

	auto entry = currentNode.lock()->children.find(name);
	if(entry == currentNode.lock()->children.end())
	{
		std::shared_ptr<profileNode> newN = std::make_shared<profileNode>(name,current,currentNode);
		currentNode.lock()->children.insert({name,newN});
		currentNode = newN;
		return;	
	}

	currentNode = entry->second;
	currentNode.lock()->calls++;
	currentNode.lock()->start = current;
}

void debugMgr::endProfiledFunc()
{
	if(debugFlags & profilerPaused)
	{
		return;
	}

	uint64 time = engine->time.get("debug") - currentNode.lock()->start;

	currentNode.lock()->heir += time;

	uint64 exceptSelf = 0;

	for(auto entry : currentNode.lock()->children)
	{
		exceptSelf += entry.second->heir;
	}

	currentNode.lock()->self += time - exceptSelf;

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
	logger.LogInfo("Set fps cap to " + std::to_string(fps));
	fpsCap = fps;
}

uint8 debugMgr::getFPSCap()
{
	return fpsCap;
}

// Terminating precompiler directives  ////////////////////////////////////////
