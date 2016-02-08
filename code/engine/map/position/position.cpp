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
	@brief adjusts chunkPos/offset if realPos is OOB
*/
void map_position::clamp()
{
	while(realPos.x < 0) {
		realPos.x += CHUNK_SIZE_METERS;
		chunkPos.x--;
		realChunkOffset.x--;
	} 
	while(realPos.x > CHUNK_SIZE_METERS) {
		realPos.x -= CHUNK_SIZE_METERS;
		chunkPos.x++;
		realChunkOffset.x++;
	}
	while(realPos.y < 0) {
		realPos.y += CHUNK_SIZE_METERS;
		chunkPos.y--;
		realChunkOffset.y--;
	}
	while(realPos.y > CHUNK_SIZE_METERS) {
		realPos.y -= CHUNK_SIZE_METERS;
		chunkPos.y++;
		realChunkOffset.y++;
	}
}

/**
	@brief compares map positions
	
	Compares values

	@param[in] comp position to compare

	@return bool positions equal
*/
bool map_position::operator==(const map_position& comp) const
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
	realChunkOffset = src.realChunkOffset;
	return *this;
}

/**
	@brief adds map positions

	Adds values

	@param[in] src position to add

	@return new added position
*/
map_position& map_position::operator+(const map_position& src) const
{
	map_position ret = *this;
	ret.realPos += src.realPos;
	ret.chunkPos += src.chunkPos;
	ret.realChunkOffset += src.chunkPos;
	ret.clamp();
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
	realChunkOffset += src.chunkPos;
	clamp();
	return *this;
}

/**
	@brief Subtracts map positions
	
	Subtracts values

	@param[in] src position to subtract

	@return self for chaining
*/
map_position& map_position::operator-(const map_position& src) const
{
	map_position ret;
	ret = *this;
	ret.realPos -= src.realPos;
	ret.chunkPos -= src.chunkPos;
	ret.realChunkOffset -= src.chunkPos;
	ret.clamp();
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
	realChunkOffset -= src.chunkPos;
	clamp();
	return *this;
}

// Terminating precompiler directives  ////////////////////////////////////////
