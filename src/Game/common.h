#pragma once

#include <common.h>

// Use the engine common for logging, assert, etc defines
	// TODO: shouldn't need to do this

///////////////////////////////////////////////////////////////////////////////

#ifdef GAME_EXPORT
	#define GAME_API declspec(dllexport)
#else
	#define GAME_API __declspec(dllimport)
#endif // ENGINE_EXPORT
