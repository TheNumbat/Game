// Program Information ////////////////////////////////////////////////////////

/**
	@file game_common.h
	@author Max Slater

	@brief Common header for the game logic containing global typedefs, macros, etc

	@version 1.00 (28 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

#include <stdint.h>

#undef assert(a)
#ifdef GAME_ASSERSIONS
	#include <assert.h>
#else
    /// Compile out assertions if ASSERTIONS not defined
    #define assert(a)
#endif

#ifdef GAME_EXPORTS
#define GAME_API __declspec(dllexport)
#else
#define GAME_API __declspec(dllimport)
#endif

// Typedefs ///////////////////////////////////////////////////////////////////

/// Typedefs for style
typedef float real32;
typedef double real64;
typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t  int64;
typedef uint64_t uint64;

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
