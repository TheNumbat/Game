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

map_position::map_position(int32 cx, int32 cy, int32 cz,
						   real32 x, real32 y, real32 z)
{
	realPos.x = x;
	realPos.y = y;
	realPos.z = z;
	chunkPos.x = cx;
	chunkPos.y = cy;
	chunkPos.z = cz;
	realChunkOffset.x = 0;
	realChunkOffset.y = 0;
	realChunkOffset.z = 0;
}

map_position::map_position(const chunk_position& c, const real_position& r)
{
	realPos = r;
	chunkPos = c;
	realChunkOffset.x = 0;
	realChunkOffset.y = 0;
	realChunkOffset.z = 0;
}

map_position::map_position(const map_position& src)
{
	realPos.x = src.realPos.x;
	realPos.y = src.realPos.y;
	realPos.z = src.realPos.z;
	chunkPos.x = src.chunkPos.x;
	chunkPos.y = src.chunkPos.y;
	chunkPos.z = src.chunkPos.z;
	realChunkOffset.x = src.realChunkOffset.x;
	realChunkOffset.y = src.realChunkOffset.y;
	realChunkOffset.z = src.realChunkOffset.z;
}

map_position::~map_position()
{

}

void map_position::clamp()
{
	while(realPos.x < 0) 
	{
		realPos.x += CHUNK_SIZE_METERS;
		chunkPos.x--;
		realChunkOffset.x--;
	} 
	while(realPos.x > CHUNK_SIZE_METERS) 
	{
		realPos.x -= CHUNK_SIZE_METERS;
		chunkPos.x++;
		realChunkOffset.x++;
	}
	while(realPos.y < 0) 
	{
		realPos.y += CHUNK_SIZE_METERS;
		chunkPos.y--;
		realChunkOffset.y--;
	}
	while(realPos.y > CHUNK_SIZE_METERS) 
	{
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

map_position map_position::operator=(const map_position& src)
{
	realPos = src.realPos;
	chunkPos = src.chunkPos;
	realChunkOffset = src.realChunkOffset;
	return *this;
}

map_position map_position::operator+(const map_position& src) const
{
	map_position ret = *this;
	ret.realPos += src.realPos;
	ret.chunkPos += src.chunkPos;
	ret.realChunkOffset += src.chunkPos;
	ret.clamp();
	return ret;
}

map_position map_position::operator+=(const map_position& src)
{
	realPos += src.realPos;
	chunkPos += src.chunkPos;
	realChunkOffset -= src.chunkPos;
	clamp();
	return *this;
}


map_position map_position::operator-(const map_position& src) const
{
	map_position ret = *this;
	ret.realPos -= src.realPos;
	ret.chunkPos -= src.chunkPos;
	ret.realChunkOffset -= src.chunkPos;
	ret.clamp();
	return ret;
}

map_position map_position::operator-=(const map_position& src)
{
	realPos -= src.realPos;
	chunkPos -= src.chunkPos;
	realChunkOffset += src.chunkPos;
	clamp();
	return *this;
}

// Terminating precompiler directives  ////////////////////////////////////////
