#pragma once

#include <common.h>

// Use the engine common for logging, assert, etc defines
	// TODO: shouldn't need to do this

///////////////////////////////////////////////////////////////////////////////

#ifdef GAME_EXPORT
	#define DLL_OUT extern "C" __declspec(dllexport)
#else
	#define DLL_OUT __declspec(dllimport)
#endif // ENGINE_EXPORT
