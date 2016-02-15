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

map_position::map_position(int32 x, int32 y, int32 z,
						   real32 cx, real32 cy, real32 cz)
	: realPos(x,y,z), chunkPos(cx,cy,cy)
{

}

map_position::map_position(const real_position& r, const chunk_position& c)
	: realPos(r), chunkPos(c)
{

}

map_position::map_position(const map_position& src)
	: realPos(src.realPos), chunkPos(src.chunkPos)
{

}

map_position::~map_position()
{

}

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

bool map_position::operator==(const map_position& comp) const
{
	return realPos == comp.realPos &&
		   chunkPos == comp.chunkPos;
}

map_position& map_position::operator=(const map_position& src)
{
	realPos = src.realPos;
	chunkPos = src.chunkPos;
	realChunkOffset = src.realChunkOffset;
	return *this;
}

map_position& map_position::operator+(const map_position& src) const
{
	map_position ret = *this;
	ret.realPos += src.realPos;
	ret.chunkPos += src.chunkPos;
	ret.realChunkOffset += src.chunkPos;
	ret.clamp();
	return ret;
}

map_position& map_position::operator+=(const map_position& src)
{
	realPos += src.realPos;
	chunkPos += src.chunkPos;
	realChunkOffset += src.chunkPos;
	clamp();
	return *this;
}

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

map_position& map_position::operator-=(const map_position& src)
{
	realPos -= src.realPos;
	chunkPos -= src.chunkPos;
	realChunkOffset -= src.chunkPos;
	clamp();
	return *this;
}

// Terminating precompiler directives  ////////////////////////////////////////
