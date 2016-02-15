// Program Information ////////////////////////////////////////////////////////

/**
	@file engine_common.h
	@author Max Slater

	@brief Common header containing global typedefs, macros, etc

	@version 1.00 (28 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

#include "engine_defines.h"
#include <stdint.h>

#ifdef ENGINE_ASSERSIONS
	#include <assert.h>
#else
    /// Compile out assertions if ASSERTIONS not defined
    #define assert(a)
#endif

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
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

static const real32 CHUNK_SIZE_METERS = 25.0f;

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
