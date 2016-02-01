// Program Information ////////////////////////////////////////////////////////

/**
	@file position.cpp

	@brief Implementation of position classes

	Position classes define position types

	@author Max Slater
	
	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "position.h"

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

/**
	@brief base position constructor

	Assigns values
	
	@param[in] _x x value
	@param[in] _y y value
	@param[in] _z z value
*/
template<typename T>
base_position<T>::base_position(T _x, T _y, T _z)
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
template<typename T>
base_position<T>::base_position(const base_position& src)
{
	x = src.x;
	y = src.y;
	z = src.z;
}

/**
	@brief base position constructor

	Does nothing
*/
template<typename T>
base_position<T>::~base_position()
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
template<typename T>
base_position<T>& base_position<T>::set(T _x, T _y, T _z)
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
template<typename T>
base_position<T>& base_position<T>::move(T _x, T _y, T _z)
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
template<typename T>	
bool base_position<T>::operator==(const base_position<T>& comp)
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
template<typename T>
base_position<T>& base_position<T>::operator=(const base_position<T>& src)
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
template<typename T>
base_position<T>& base_position<T>::operator+(const base_position<T>& src)
{
	return base_position(x + src.x, y + src.y, z + src.z);
}

/**
	@brief adds base positions

	Adds values

	@param[in] src position to add

	@return self for chaining
*/
template<typename T>
base_position<T>& base_position<T>::operator+=(const base_position<T>& src)
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
template<typename T>
base_position<T>& base_position<T>::operator-(const base_position<T>& src)
{
	return base_position(x - src.x, y - src.y, z - src.z);
}

/**
	@brief subtracts base positions

	Subtracts values

	@param[in] src position to subtract

	@return self for chaining
*/
template<typename T>
base_position<T>& base_position<T>::operator-=(const base_position<T>& src)
{
	x -= src.x;
	y -= src.y;
	z -= src.z;
	return *this;
}

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
map_position::map_position(int32 x, int32 y, int32 z,
						   real32 cx, real32 cy, real32 cz)
	: realPos(x,y,z), chunkPos(cx,cy,cy)
{

}

/**
	@brief map position constructor

	Assigns values

	@param[in] r real position
	@param[in] c chunk position
*/
map_position::map_position(const real_position& r, const chunk_position& c)
	: realPos(r), chunkPos(c)
{

}

/**
	@brief map position constructor

	Assigns values

	@param[in] src position to copy
*/
map_position::map_position(const map_position& src)
	: realPos(src.realPos), chunkPos(src.chunkPos)
{

}

/**
	@brief map position destructor

	Does nothing
*/
map_position::~map_position()
{

}

/**
	@brief compares map positions
	
	Compares values

	@param[in] comp position to compare

	@return bool positions equal
*/
bool map_position::operator==(const map_position& comp)
{
	return realPos == comp.realPos &&
		   chunkPos == comp.chunkPos;
}

/**
	@brief assigns map positions

	Assigns values

	@param[in] src position to assign

	@return self for chaining
*/
map_position& map_position::operator=(const map_position& src)
{
	realPos = src.realPos;
	chunkPos = src.chunkPos;
	return *this;
}

/**
	@brief adds map positions

	Adds values

	@param[in] src position to add

	@return new added position
*/
map_position& map_position::operator+(const map_position& src)
{
	map_position ret = *this;
	ret.realPos += src.realPos;
	ret.chunkPos += src.chunkPos;
	return ret;
}

/**
	@brief adds map positions

	Adds values

	@param[in] src position to add

	@return self for chaining
*/
map_position& map_position::operator+=(const map_position& src)
{
	realPos += src.realPos;
	chunkPos += src.chunkPos;
	return *this;
}

/**
	@brief Subtracts map positions
	
	Subtracts values

	@param[in] src position to subtract

	@return self for chaining
*/
map_position& map_position::operator-(const map_position& src)
{
	map_position ret;
	ret = *this;
	ret.realPos -= src.realPos;
	ret.chunkPos -= src.chunkPos;
	return ret;
}

/**
	@brief Subtracts map positions

	Subtracts values

	@param[in] src position to subtract

	@return self for chaining
*/
map_position& map_position::operator-=(const map_position& src)
{
	realPos -= src.realPos;
	chunkPos -= src.chunkPos;
	return *this;
}

// Terminating precompiler directives  ////////////////////////////////////////
