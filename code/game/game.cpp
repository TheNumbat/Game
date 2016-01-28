// Program Information ////////////////////////////////////////////////////////

/**
	@file game.cpp
	@author Max Slater

	@brief "main" for the game code, contains main game loop

	The engine's main function calls into this to run the actual game loop,
	as well as anything that must be done when the game code is loaded or freed

	@version 1.00 (27 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

/// Define a function to be exported in the game DLL
#define DllExport extern "C" __declspec(dllexport)

// Header files ///////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

// Global constant definitions  ///////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

DllExport void onLoad();
DllExport void onFree();
DllExport bool gameLoop();

// Free function implementation  //////////////////////////////////////////////

DllExport void onLoad() {

}

DllExport void onFree() {

}

DllExport bool gameLoop() {
	cout << "gameLoop" << endl;
	return true;
}

// Terminating precompiler directives  ////////////////////////////////////////
