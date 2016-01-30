// Program Information ////////////////////////////////////////////////////////

/**
	@file main.cpp

	@brief Contains main function of program, manages dynamic code reloading.
	
	Loads the game code, allocates a state, and calls into the game logic
	functions. Each frame it will test if the code needs to be reloaded and
	will do so if needed.

	@author Max Slater
	@version 1.00 (27 August 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include "log/logMgr.h"

#include <windows.h>
#include <SDL.h>
#include <string>
#include <iostream>
        using namespace std;

// Global constant definitions  ///////////////////////////////////////////////

/// Pointer type for gameLoop function
typedef bool (*CreateGameLoop)();
/// Pointer type for onLoad function
typedef void (*CreateOnLoad)();
/// Pointer type for onFree function
typedef void (*CreateOnFree)();

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Holds neccessary data for reloading the game code, as well as pointers
		   to the game code functions.
*/
struct libData 
{
	/// Pointer to SDL Object handle (Game code DLL)
	void* dllHandle;	
	/// Path to Game.dll	
	std::string SFile;	
	/// Path to be used for GameTemp.dll	
	std::string DFile;

	/// Pointer to gameLoop function
	CreateGameLoop gameLoop;
	/// Pointer to onLoad function	
	CreateOnLoad onLoad;	
	/// Pointer to onFree function	
	CreateOnFree onFree;		

	/// Logger for game code reloading
	logMgr logger;
};

// Free function prototypes  //////////////////////////////////////////////////

bool reloadLib(libData& data);

// Free function implementation  //////////////////////////////////////////////

/**
	@brief Main function of program
	
	Allocates and stores an instance of the game state, runs the game loop,
	and will reload the game code DLL on any file update.

	@param[in] argc Number of command line arguments -- not used
	@param[in] argv Array of c-style strings representing the command
			   line arguments -- not used
	
	@return int sucess, 0 = successful.

	@exception Assertions will fail if DLL or its functions fail to load properly
*/
int main(int argc, char** argv)
{
	libData data = {};

	data.logger.StartLog("MAIN",true);
	data.logger.LogDefault("Hello world!");

	// Get file names
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, sizeof(buffer));
	data.SFile = buffer;
	data.SFile = data.SFile.substr(0,data.SFile.find_last_of('\\') + 1);
	data.DFile = data.SFile + "GameTemp.dll";
	data.SFile = data.SFile + "Game.dll";

	// Copy Game.dll to GameTemp.dll -- makes sure Game.dll is not locked so
	// we can reload from it
	bool result = CopyFile(data.SFile.c_str(),data.DFile.c_str(),false);
	assert(result);

	// Load GameTemp.dll
	data.dllHandle = SDL_LoadObject(data.DFile.c_str());
	assert(data.dllHandle);

	// Load functions
	data.gameLoop = (CreateGameLoop)SDL_LoadFunction(data.dllHandle,"gameLoop");
	data.onLoad = (CreateOnLoad)SDL_LoadFunction(data.dllHandle,"onLoad");
	data.onFree = (CreateOnFree)SDL_LoadFunction(data.dllHandle,"onFree");

	assert(data.gameLoop && data.onLoad && data.onFree	);

	data.logger.LogInfo("Game code loaded.");

	// Loaded
	(*data.onLoad)();

	// Run game loop
	while((*data.gameLoop)()) {
		reloadLib(data);
	}

	return 0;
}

/**
	@brief Dynamically reloads game code
	
	Checks if the Game DLL has been written to since the last reload, and if
	it has will reload the game DLL and functions.

	@param[in] libData& contains needed data to reload code
	
	@return bool sucess

	@exception Assertions will fail if DLL or its functions fail to load properly
*/
bool reloadLib(libData& data) {

	// Find last write time to Game.dll
	WIN32_FILE_ATTRIBUTE_DATA FData;
	
	bool result = GetFileAttributesEx(data.SFile.c_str(),GetFileExInfoStandard,&FData);
	assert(result);

	static FILETIME LastWriteTime = FData.ftLastWriteTime;

	// If file has changed
	if(CompareFileTime(&FData.ftLastWriteTime,&LastWriteTime) == 1) {

		LastWriteTime = FData.ftLastWriteTime;

		// Free DLL
		(*data.onFree)();
		SDL_UnloadObject(data.dllHandle);

		// Copy to temp DLL
		while(!CopyFile(data.SFile.c_str(),data.DFile.c_str(),false)) {
			/// @todo make waiting for temp file to unlock better -- maybe generate
			/// GameTemp + reloadNum.dll each time
		}

		// Load new temp DLL
		data.dllHandle = SDL_LoadObject(data.DFile.c_str());
		assert(data.dllHandle);

		// Load functions
		data.gameLoop = (CreateGameLoop)SDL_LoadFunction(data.dllHandle,"gameLoop");
		data.onLoad = (CreateOnLoad)SDL_LoadFunction(data.dllHandle,"onLoad");
		data.onFree = (CreateOnFree)SDL_LoadFunction(data.dllHandle,"onFree");
		assert(data.gameLoop && data.onLoad && data.onFree);

		// Loaded
		(*data.onLoad)();

		data.logger.LogInfo("Reloaded game code.");
	}

	return true;
}

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
