// Program Information ////////////////////////////////////////////////////////

/**
	@file main.cpp
	@author Max Slater

	@brief Contains main function of program, manages dynamic code reloading.
	
	<extensive explanation>

	@version 1.00 (27 August 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include <windows.h>
#include <SDL.h>
#include <string>
#include <assert.h>

// Global constant definitions  ///////////////////////////////////////////////

typedef bool (*CreateGameLoop)();
typedef void (*CreateOnLoad)();
typedef void (*CreateOnFree)();

// Class/Struct definitions  //////////////////////////////////////////////////

struct libData {
	void* dllHandle;
	std::string SFile;
	std::string DFile;

	CreateGameLoop gameLoop;
	CreateOnLoad onLoad;
	CreateOnFree onFree;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

/**
	@brief Main function of program
	
	Allocates and stores an instance of the game state, runs the game loop,
	and will reload the game code DLL on any file update.

	@param[in] argc Number of command line arguments -- not used
	@param[in] argv Array of c-style strings representing the command
			   line arguments -- not used
	
	@return int sucess, 0 = successful.

	@exception Will fail and quit if DLL or its functions fail to load properly
*/
int main(int argc, char** argv)
{
	libData data = {};

	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, sizeof(buffer));
	data.SFile = buffer;
	data.SFile = data.SFile.substr(0,data.SFile.find_last_of('\\') + 1);
	data.DFile = data.SFile + "GameTemp.dll";
	data.SFile = data.SFile + "Game.dll";

	assert(CopyFile(data.SFile.c_str(),data.DFile.c_str(),false));

	data.dllHandle = SDL_LoadObject(data.DFile.c_str());
	assert(data.dllHandle);

	data.gameLoop = (CreateGameLoop)SDL_LoadFunction(data.dllHandle,"gameLoop");
	data.onLoad = (CreateOnLoad)SDL_LoadFunction(data.dllHandle,"onLoad");
	data.onFree = (CreateOnFree)SDL_LoadFunction(data.dllHandle,"onFree");

	assert(data.gameLoop && data.onLoad && data.onFree);
	
	while((*data.gameLoop)()) {}

	return 0;
}

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
