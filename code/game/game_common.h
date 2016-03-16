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
/// Expose functions in library
#define GAME_API __declspec(dllexport)
#else
/// Don't expose functions in library
#define GAME_API __declspec(dllimport)
#endif

/// Load font
#define LOAD_FONT(path,ID,size) engine->graphics.loadFont(path,ID,size)
/// Load texture
#define LOAD_TEXTURE(path,ID) engine->graphics.loadTexture(path,ID)
/// Load sound
#define LOAD_SOUND(path,ID) engine->audio.loadSound(path,ID)
/// Load file
#define LOAD_FILE(path,type,access,ID) engine->file.loadFile(path,type,access,ID) 
/// Load lib
#define LOAD_LIB(path,ID) engine->file.loadLibrary(path,ID)

/// Free font
#define FREE_FONT(path) engine->graphics.freeFont(ID)
/// Free texture
#define FREE_TEXTURE(path) engine->graphics.freeTexture(ID)
/// Free sound
#define FREE_SOUND(path) engine->audio.freeSound(ID)
/// Free file
#define FREE_FILE(path) engine->file.freeFile(ID) 
/// Free lib
#define FREE_LIB(path) engine->file.freeLibrary(ID)

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
