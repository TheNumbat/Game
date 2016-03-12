// Program Information ////////////////////////////////////////////////////////

/**
	@file modMgr.cpp

	@brief Implementation file for the modMgr class.

	@author Max Slater

	@version 1.00 (6 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "modMgr.h"

#include "game_state.h"
#include <engine_state.h>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

modMgr::modMgr(game_state* g, engine_state* e)
{
	game = g;
	engine = e;
	logger.StartLog("MODS");
}

modMgr::~modMgr()
{
	for(auto& name : modLibIDs)
	{
		engine->file.freeLibrary(name);
	}
}

void modMgr::startup()
{
	modLibIDs = engine->file.getNames("mods");
	engine->file.loadLibraryRec("mods");
	for(auto& name : modLibIDs)
	{
		void* func;

		func = engine->file.loadFunction(name,"startup");
		if(!func)
		{
			logger.LogWarn("Could not load startup function from mod ID " + name);
			modLibIDs.erase(std::find(modLibIDs.begin(), modLibIDs.end(), name));
			continue;
		}
		startupFuncs.push_back((startupFunc)func);

		func = engine->file.loadFunction(name,"update");
		if(!func)
		{
			logger.LogWarn("Could not load update function from mod ID " + name);
			modLibIDs.erase(std::find(modLibIDs.begin(), modLibIDs.end(), name));
			startupFuncs.pop_back();
			continue;
		}
		updateFuncs.push_back((updateFunc)func);

		func = engine->file.loadFunction(name,"shutdown");
		if(!func)
		{
			logger.LogWarn("Could not load shutdown function from mod ID " + name);
			modLibIDs.erase(std::find(modLibIDs.begin(), modLibIDs.end(), name));
			startupFuncs.pop_back();
			updateFuncs.pop_back();
			continue;
		}
		shutdownFuncs.push_back((shutdownFunc)func);
	}

	for(auto& func : startupFuncs)
	{
		(*func)(game,engine);
	}

	logger.LogInfo("Mod code initialized");
}

void modMgr::updateMods()
{
	game->debug.beginProfiledFunc();

	for(auto& func : updateFuncs)
	{
		(*func)(game,engine);
	}

	game->debug.endProfiledFunc();
}

void modMgr::shutdown()
{
	for(auto& func : shutdownFuncs)
	{
		(*func)(game,engine);
	}
}

// Terminating precompiler directives  ////////////////////////////////////////
