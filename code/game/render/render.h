// Program Information ////////////////////////////////////////////////////////

/**
	@file render.h
	@author Max Slater

	@brief does rendering for the game, called in gameLoop

	@version 1.00 (15 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

/// Rendering macros
#define PIXELS_TO_METERS (1.0f / METERS_TO_PIXELS)
#define CHUNK_SIZE_PIXELS (CHUNK_SIZE_METERS * METERS_TO_PIXELS)

// Header files ///////////////////////////////////////////////////////////////

#include "game_common.h"
#include <engine_state.h>

#include <algorithm>
#include <vector>
#include <string>
#include <cmath>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Describes data needed to output a texture

	Used as an intermediate between entity textures and graphicMgr calls
*/
struct rawTexture
{
	std::string ID;
	v2<real32> pixelPos;
	v2<real32> pixelDim;
	bool forceTop;
	bool forceBot;
};

// Free function prototypes  //////////////////////////////////////////////////

/**
	@brief Renders the game map

	@param[in] engine pointer to the engine state
	@param[in] game pointer to the game state
*/
void renderMap(engine_state* engine, game_state* game);

/**
	@brief Maps a point into pixel space, using another point as the origin

	@param[in] origin position to use as the origin
	@param[in] point position to map
	@param[in] zoom current camera zoom

	@return relative pixel position
*/
v2<real32>& mapIntoPixelSpace(const map_position& origin, const map_position& point, real32 zoom);

/**
	@brief Sorts an array of rawTexture structures by their y position and forceTop/Bot

	@param[in] textures vector of rawTexture to sort
*/
void sortTextures(std::vector<rawTexture>& textures);

// Free function implementation  //////////////////////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
