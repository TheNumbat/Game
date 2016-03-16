// Program Information ////////////////////////////////////////////////////////

/**
	@file engine_common.h
	@author Max Slater

	@brief Common engine header containing global typedefs, macros, etc

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
/// Expose functions in library
#define ENGINE_API __declspec(dllexport)
#else
/// Do not expose functions in library
#define ENGINE_API __declspec(dllimport)
#endif

// Typedefs ///////////////////////////////////////////////////////////////////

/// For style
typedef float real32; /// For style 
typedef double real64; /// For style
typedef int8_t int8; /// For style
typedef uint8_t uint8; /// For style
typedef int16_t int16; /// For style
typedef uint16_t uint16; /// For style
typedef int32_t int32; /// For style
typedef uint32_t uint32; /// For style
typedef int64_t  int64; /// For style
typedef uint64_t uint64;

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
