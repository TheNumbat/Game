// Program Information ////////////////////////////////////////////////////////

/**
	@file rendering.cpp
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

void renderMap(engine_state* engine, game_state* game)
{
	std::vector<rawTexture> textures;

	// Get window size
	int32 winW, winH;
	engine->graphics.getWinDim(winW,winH);

	// Set up varaibles describing render region
	real32 camZoom = game->cam.zoom;
	real32 halfScreenWidthMeters = (winW / 2.0f) * (PIXELS_TO_METERS / camZoom);
	real32 halfScreenHeightMeters = (winH / 2.0f) * (PIXELS_TO_METERS / camZoom);

	// Camrea position is the center of the screen, so this gives us a position at the TLC to render from
	map_position centerPos = game->cam.getCenter();
	map_position offset = map_position(0,0,0,halfScreenWidthMeters,halfScreenHeightMeters,0);
	map_position TLCpos = centerPos - offset;
	map_position BRCpos = centerPos + offset;

	// Find region of chunks
	int32 relChunkXMin = TLCpos.realChunkOffset.x;
	int32 relChunkXMax = BRCpos.realChunkOffset.x;

	int32 relChunkYMin = TLCpos.realChunkOffset.y;
	int32 relChunkYMax = BRCpos.realChunkOffset.y;

	// Iterate through region of chunks
	for(int32 relX = relChunkXMin; relX <= relChunkXMax; relX++)
	{
		for(int32 relY = relChunkYMin; relY <= relChunkYMax; relY++)
		{
			// Get chunk
			chunk_position currentChunkPos = game->cam.getCenter().chunkPos + chunk_position(relX,relY,0);

			std::weak_ptr<chunk> currentChunk = engine->map.getChunk(currentChunkPos);

			// If chunk exists
			if(!currentChunk.expired())
			{
				// Debug: draw chunk boundry
				#ifdef DRAW_CHUNK_BOUNDS
				{
					v2<real32> pixelPos = mapIntoPixelSpace( TLCpos, map_position( currentChunkPos, real_position(0,0,0) ), camZoom );
					rect2<int32> pixelChunk( floor(pixelPos.x), floor(pixelPos.y), ceil(CHUNK_SIZE_PIXELS * camZoom), ceil(CHUNK_SIZE_PIXELS * camZoom) );
					engine->graphics.renderTexture("chunkbounds", pixelChunk);
				}
				#endif

				#ifdef DRAW_CAMERA 
				{
					v2<real32> pixelPos = mapIntoPixelSpace( TLCpos, centerPos, camZoom );
					rect2<int32> pixelChunk( floor(pixelPos.x) - (0.5 * METERS_TO_PIXELS * camZoom), floor(pixelPos.y) - (0.5 * METERS_TO_PIXELS * camZoom), ceil(METERS_TO_PIXELS * camZoom), ceil(METERS_TO_PIXELS * camZoom) );
					engine->graphics.renderTexture("camera", pixelChunk);
				}
				#endif

				for(std::weak_ptr<entity> e : *currentChunk.lock())
				{
					std::lock_guard<std::mutex> lock(e.lock()->lock);

					// Test if entity can be rendered
					if(e.lock()->hasComponent(ctype_position) && e.lock()->hasComponent(ctype_texture))
					{
						// Get components
						std::weak_ptr<component_position> ePosition = std::static_pointer_cast<component_position>(e.lock()->getComponent(ctype_position).lock());
						std::weak_ptr<component_texture> eTexture = std::static_pointer_cast<component_texture>(e.lock()->getComponent(ctype_texture).lock());

						// Get each texture from the entity
						for(int32 texIndex = 0; texIndex < eTexture.lock()->textureIDs.size(); texIndex++)
						{
							// Get texture information
							std::string texID = eTexture.lock()->textureIDs[texIndex];
							v2<real32> texPos = eTexture.lock()->texturePositions[texIndex];
							v2<real32> texDim = eTexture.lock()->textureDimensions[texIndex];

							// Map the texture into pixel space (against TLC of window)
							/// @todo z-space
							v2<real32> entityPixelPos = mapIntoPixelSpace(TLCpos,ePosition.lock()->pos,camZoom);
							v2<real32> texPixelPos = entityPixelPos + ( texPos * METERS_TO_PIXELS * camZoom);

							// Create new raw texture to render
							rawTexture texture;
							texture.ID = texID;
							texture.pixelPos = texPixelPos;
							texture.pixelDim = texDim * METERS_TO_PIXELS * camZoom;
							texture.forceTop = eTexture.lock()->forceTop;
							texture.forceBot = eTexture.lock()->forceBot;

							textures.push_back(texture);
						}
					}
				}
			}
		}		
	}

	// Sort textures
	sortTextures(textures);

	// Actually render textures
	for(rawTexture t : textures) 
	{
		engine->graphics.renderTexture(t.ID, rect2<int32>( floor(t.pixelPos.x), floor(t.pixelPos.y), ceil(t.pixelDim.x), ceil(t.pixelDim.y)));
	}
}

v2<real32>& mapIntoPixelSpace(const map_position& origin, const map_position& point, real32 zoom)
{
	// Calculate relative chunk and within-chunk positions
	v2<real32> relDistance( point.realPos.x - origin.realPos.x , point.realPos.y - origin.realPos.y );
	v2<real32> chunkDistance( point.chunkPos.x - origin.chunkPos.x , point.chunkPos.y - origin.chunkPos.y );

	// Calculate total distance
	v2<real32> totalDistanceMeters = ( chunkDistance * CHUNK_SIZE_METERS ) + relDistance;
	v2<real32> totalDistancePixels = totalDistanceMeters * METERS_TO_PIXELS * zoom;

	return totalDistancePixels;
}

void sortTextures(std::vector<rawTexture>& textures)
{
	// Sort based on y-position
	std::sort(textures.begin(), textures.end(), 
	    [](const rawTexture& a, const rawTexture& b) -> bool
	{ 
	    return (a.pixelPos.y + a.pixelDim.y) < (b.pixelPos.y + b.pixelDim.y); 
	});

	// Account for forceTop/Bot
	int size = textures.size();
	for(int i = 0; i < size; i++)
	{
		if(textures[i].forceTop) 
		{
			textures.push_back(textures[i]);
			textures.erase(textures.begin() + i);
		} 
		else if(textures[i].forceBot) 
		{
			textures.insert(textures.begin(),textures[i]);
			textures.erase(textures.begin() + i + 1);
		}
	}
}

// Terminating precompiler directives  ////////////////////////////////////////
