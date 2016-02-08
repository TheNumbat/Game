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

#include <functional>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Describes a position

	Used to make poth real32 per-chunk positions and aboslute chunk
	positions

	Used by mapMgr and elsewhere.
*/
template<typename T>
struct base_position
{
	/**
		@brief base position constructor

		Assigns values
		
		@param[in] _x x value
		@param[in] _y y value
		@param[in] _z z value
	*/
	ENGINE_API base_position(T _x = 0, T _y = 0, T _z = 0)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	/**
		@brief base position constructor

		Assigns values

		@param[in] src position to copy
	*/
	ENGINE_API base_position(const base_position<T>& src)
	{
		x = src.x;
		y = src.y;
		z = src.z;
	}

	/**
		@brief base position constructor

		Does nothing
	*/
	ENGINE_API ~base_position()
	{

	}

	/**
		@brief set base position

		Assigns values

		@param[in] _x x value
		@param[in] _y y value
		@param[in] _z z value

		@return self for chaining
	*/
	ENGINE_API base_position<T>& set(T _x, T _y, T _z)
	{
		x = _x;
		y = _y;
		z = _z;
		return *this;
	}

	/**
		@brief Move base position

		Adds values

		@param[in] _x x value
		@param[in] _y y value
		@param[in] _z z value

		@return self for chaining
	*/
	ENGINE_API base_position<T>& move(T _x, T _y, T _z)
	{
		x += _x;
		y += _y;
		z += _z;
		return *this;
	}

	/**
		@brief compares base positions

		Compares values

		@param[in] comp position to compare to

		@return bool equal
	*/
	ENGINE_API bool operator==(const base_position<T>& comp) const
	{
		return x == comp.x &&
			   y == comp.y &&
			   z == comp.z;
	}

	/**
		@brief assigns base positions

		Assigns values

		@param[in] src position to assign

		@return self for chaining
	*/
	ENGINE_API base_position<T>& operator=(const base_position<T>& src)
	{
		x = src.x;
		y = src.y;
		z = src.z;
		return *this;
	}

	/**
		@brief adds base positions

		Adds values

		@param[in] src position to add

		@return new added position
	*/
	ENGINE_API base_position<T>& operator+(const base_position<T>& src) const
	{
		return base_position(x + src.x, y + src.y, z + src.z);
	}

	/**
		@brief adds base positions

		Adds values

		@param[in] src position to add

		@return self for chaining
	*/
	ENGINE_API base_position<T>& operator+=(const base_position<T>& src)
	{
		x += src.x;
		y += src.y;
		z += src.z;
		return *this;
	}

	/**
		@brief subtracts base positions

		Subtracts values

		@param[in] src position to subtract

		@return new subtracted position
	*/
	ENGINE_API base_position<T>& operator-(const base_position<T>& src) const
	{
		return base_position(x - src.x, y - src.y, z - src.z);
	}

	/**
		@brief subtracts base positions

		Subtracts values

		@param[in] src position to subtract

		@return self for chaining
	*/
	ENGINE_API base_position<T>& operator-=(const base_position<T>& src)
	{
		x -= src.x;
		y -= src.y;
		z -= src.z;
		return *this;
	}

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

	ENGINE_API bool operator==(const map_position& comp) const;
	ENGINE_API map_position& operator=(const map_position& src);

	ENGINE_API map_position& operator+(const map_position& src) const;
	ENGINE_API map_position& operator+=(const map_position& src);
	ENGINE_API map_position& operator-(const map_position& src) const;
	ENGINE_API map_position& operator-=(const map_position& src);

	ENGINE_API void clamp();

	real_position realPos;
	chunk_position chunkPos;
	chunk_position realChunkOffset;
};

/**
	@brief Hash function for chunk_position

	Used for sparsely storing "chunks" (see mapMgr)
*/
namespace std
{
	template<> 
	struct hash<chunk_position>
	{
		uint64 operator()(const chunk_position& hash_pos) const
		{
			/// @todo do we need to call the other hash functors?
            uint64 h1 = std::hash<int32>()(hash_pos.x);
            uint64 h2 = std::hash<int32>()(hash_pos.y);
            uint64 h3 = std::hash<int32>()(hash_pos.z);

            return h1 ^ ((h2 ^ (h3 << 1)) << 1);
		}
	};
}

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
