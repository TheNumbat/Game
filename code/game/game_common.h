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

#define LOAD_FONT(path,ID,size) engine->graphics.loadFont(path,ID,size)
#define LOAD_TEXTURE(path,ID) engine->graphics.loadTexture(path,ID)
#define LOAD_SOUND(path,ID) engine->audio.loadSound(path,ID)
#define LOAD_FILE(path,type,access,ID) engine->file.loadFile(path,type,access,ID) 
#define LOAD_LIB(path,ID) engine->file.loadLibrary(path,ID)

#define FREE_FONT(path) engine->graphics.freeFont(ID)
#define FREE_TEXTURE(path) engine->graphics.freeTexture(ID)
#define FREE_SOUND(path) engine->audio.freeSound(ID)
#define FREE_FILE(path) engine->file.freeFile(ID) 
#define FREE_LIB(path) engine->file.freeLibrary(ID)

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
