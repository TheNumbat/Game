// Program Information ////////////////////////////////////////////////////////

/**
	@file position.h

	@brief Declartion file for position classes

	incldues map_position, chunk_position, real_position

	@author Max Slater

	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Describes a floating point position

	Used for defining position within a chunk

	Used by mapMgr and elsewhere.
*/
template<typename T>
struct base_position
{
	ENGINE_API base_position(T _x = 0, T _y = 0, T _z = 0);
	ENGINE_API base_position(const base_position& src);
	ENGINE_API ~base_position();

	ENGINE_API base_position& set(T _x, T _y, T _z);
	ENGINE_API base_position& move(T _x, T _y, T _z);

	ENGINE_API bool operator==(const base_position& comp);
	ENGINE_API base_position& operator=(const base_position& src);

	ENGINE_API base_position& operator+(const base_position& src);
	ENGINE_API base_position& operator+=(const base_position& src);
	ENGINE_API base_position& operator-(const base_position& src);
	ENGINE_API base_position& operator-=(const base_position& src);

	T x;
	T y;
	T z;
};

/// Real position for use relative to chunk
typedef base_position<real32> real_position;
/// Chunk position for aboslute chunk location
typedef base_position<int32> chunk_position;

/**
	@brief Describes a map position, i.e. both chunk and real position

	Used for defining absolute map position

	Used by mapMgr and elsewhere.
*/
struct map_position
{
	ENGINE_API map_position(int32 x = 0, int32 y = 0, int32 z = 0,
							real32 cx = 0, real32 cy = 0, real32 cz = 0);
	ENGINE_API map_position(const real_position& r, const chunk_position& c);
	ENGINE_API map_position(const map_position& src);
	ENGINE_API ~map_position();

	ENGINE_API bool operator==(const map_position& comp);
	ENGINE_API map_position& operator=(const map_position& src);

	/// @todo do we still want these to move around the chunk pos
	/// if realPos is out of bounds? Then we need chunkDiff again
	ENGINE_API map_position& operator+(const map_position& src);
	ENGINE_API map_position& operator+=(const map_position& src);
	ENGINE_API map_position& operator-(const map_position& src);
	ENGINE_API map_position& operator-=(const map_position& src);

	real_position realPos;
	chunk_position chunkPos;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
