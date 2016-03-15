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

#include "game_common.h"

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
	GAME_API base_position(T _x = (T)0, T _y = (T)0, T _z = (T)0)
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
	GAME_API base_position(const base_position& src)
	{
		x = src.x;
		y = src.y;
		z = src.z;
	}

	/**
		@brief base position constructor

		Does nothing
	*/
	GAME_API ~base_position()
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
	GAME_API base_position& set(T _x, T _y, T _z)
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
	GAME_API base_position& move(T _x, T _y, T _z)
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
	GAME_API bool operator==(const base_position& comp) const
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
	GAME_API base_position operator=(const base_position& src)
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
	GAME_API base_position operator+(const base_position& src) const
	{
		return base_position(x + src.x, y + src.y, z + src.z);
	}

	/**
		@brief adds base positions

		Adds values

		@param[in] src position to add

		@return self for chaining
	*/
	GAME_API base_position operator+=(const base_position& src)
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
	GAME_API base_position operator-(const base_position& src) const
	{
		return base_position(x - src.x, y - src.y, z - src.z);
	}

	/**
		@brief subtracts base positions

		Subtracts values

		@param[in] src position to subtract

		@return self for chaining
	*/
	GAME_API base_position operator-=(const base_position& src)
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
	/**
		@brief map position constructor

		Assigns values

		@param[in] x x real value
		@param[in] y y real value
		@param[in] z z real value
		@param[in] cx x chunk value
		@param[in] cy y chunk value
		@param[in] cz z chunk value
	*/
	GAME_API map_position(int32 cx = 0, int32 cy = 0, int32 cz = 0,
						  real32 x = 0, real32 y = 0, real32 z = 0);

	/**
		@brief map position constructor

		Assigns values

		@param[in] r real position
		@param[in] c chunk position
	*/
	GAME_API map_position(const chunk_position& c, const real_position& r);

	/**
		@brief map position constructor

		Assigns values

		@param[in] src position to copy
	*/
	GAME_API map_position(const map_position& src);

	/**
		@brief map position destructor

		Does nothing
	*/
	GAME_API ~map_position();

	/**
		@brief compares map positions
		
		Compares values

		@param[in] comp position to compare

		@return bool positions equal
	*/
	GAME_API bool operator==(const map_position& comp) const;

	/**
		@brief assigns map positions

		Assigns values

		@param[in] src position to assign

		@return self for chaining
	*/
	GAME_API map_position operator=(const map_position& src);

	/**
		@brief adds map positions

		Adds values

		@param[in] src position to add

		@return new added position
	*/
	GAME_API map_position operator+(const map_position& src) const;

	/**
		@brief adds map positions

		Adds values

		@param[in] src position to add

		@return self for chaining
	*/
	GAME_API map_position operator+=(const map_position& src);

	/**
		@brief Subtracts map positions
		
		Subtracts values

		@param[in] src position to subtract

		@return self for chaining
	*/
	GAME_API map_position operator-(const map_position& src) const;

	/**
		@brief Subtracts map positions

		Subtracts values

		@param[in] src position to subtract

		@return self for chaining
	*/
	GAME_API map_position operator-=(const map_position& src);

	/**
		@brief adjusts chunkPos/offset if realPos is out of bounds
	*/
	GAME_API void clamp();

	real_position realPos;
	chunk_position chunkPos;
	chunk_position realChunkOffset;
};

/**
	@brief Currently only used to add hash function for chunk_position
*/
namespace std
{
	/**
		@brief Hash function for chunk_position

		Used for sparsely storing "chunks" (see mapMgr)
	*/
	template<> 
	struct hash<chunk_position>
	{
		uint64 operator()(const chunk_position& hash_pos) const
		{
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
