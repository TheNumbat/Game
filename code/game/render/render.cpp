// Program Information ////////////////////////////////////////////////////////

/**
	@file render.cpp
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

#include "render.h"

#include "game_state/game_state.h"

#include <vect.h>
#include <engine_state.h>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

struct rawTex
{
	virtual ~rawTex() = 0 {};
	rect2<real32> pixelRect;
	blendmode blend;
	color mod;
	uint32 layer;

	rect2<int32> srcPixelRect;
	v2<real32> rotPoint;
	real32 rot;
	uint32 flip;
};

/**
	@brief Describes data needed to output a texture

	Used as an intermediate between entity textures and graphicMgr calls
*/
struct rawTexture : public rawTex
{
	rawTexture() {};
	rawTexture(const std::string& tID, const rect2<real32>& rect, blendmode b, color c, uint32 l,
			   const rect2<int32>& spr, const v2<real32>& rpt, real32 r, uint32 f)
	{
		ID = tID;
		pixelRect = rect;
		blend = b;
		mod = c;
		layer = l;
		srcPixelRect = spr;
		rotPoint = rpt;
		rot = r;
		flip = f;
	}
	rawTexture(const component_texture::sub_texture& t, real32 zoom, const v2<real32>& offset)
	{
		ID = t.texID;
		pixelRect = (t.texRect * METERS_TO_PIXELS * zoom) + offset;
		rotPoint = t.rotPt * METERS_TO_PIXELS * zoom;
		blend = t.blend;
		mod = t.mod;
		layer = t.layer;
		srcPixelRect = t.srcPixelRect;
		rot = t.rot;
		flip = t.flip;
	}
	std::string ID;
};

/**
	@brief Describes data needed to output a text texture

	Used as an intermediate between entity textures and graphicMgr calls
*/
struct rawText : public rawTex
{
	rawText() {};
	rawText(const std::string& fID, const std::string& message, const rect2<real32>& rect, blendmode b, color c, uint32 l,
		    const rect2<int32>& spr, const v2<real32>& rpt, real32 r, uint32 f)
	{
		fontID = fID;
		text = message;
		pixelRect = rect;
		blend = b;
		mod = c;
		layer = l;
		srcPixelRect = spr;
		rotPoint = rpt;
		rot = r;
		flip = f;
	}
	rawText(const component_text_texture::sub_text_texture& t, real32 zoom, const v2<real32>& offset)
	{
		fontID = t.fontID;
		text = t.message;
		pixelRect = ( t.texRect * METERS_TO_PIXELS * zoom) + offset;
		rotPoint = t.rotPt * METERS_TO_PIXELS * zoom;
		blend = t.blend;
		mod = t.mod;
		layer = t.layer;
		srcPixelRect = t.srcPixelRect;
		rot = t.rot;
		flip = t.flip;
	}
	std::string fontID;
	std::string text;
};

// Free function prototypes  //////////////////////////////////////////////////

/**
	@brief Renders the game HUD

	@param[in] engine pointer to the engine state
	@param[in] game pointer to the game state
*/
void renderHUD(engine_state* engine, game_state* game);

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
void sortTextures(std::vector<rawTex*>& textures);

/**
	@brief Gets all textures from the game map (that can be seen)

	@param[in] engine pointer to the engine state
	@param[in] game pointer to the game state
	@param[out] textures vector to be filled with textures
*/
void getMapTextures(engine_state* engine, game_state* game, std::vector<rawTex*>& textures);

/**
	@brief Renderes any number of rawTex textures, then deletes/clears the array

	@param[in] engine pointer to the engine state
	@param[in] textures to render (can be rawTexture or rawText)
*/
void renderAndClearTextures(engine_state* engine, std::vector<rawTex*>& textures);

/**
	@brief Renderes the debug info for the game

	@param[in] engine pointer to the engine state
	@param[in] game pointer to the game state
*/
void renderDebugUI(engine_state* engine, game_state* game);

// Free function implementation  //////////////////////////////////////////////

void render(engine_state* engine, game_state* game)
{
	renderMap(engine,game);
	renderHUD(engine,game);

	if(game->showDebugUI)
	{
		renderDebugUI(engine,game);
	}

	engine->graphics.displayFrame();
}

void renderHUD(engine_state* engine, game_state* game)
{
	int32 winW, winH;
	engine->graphics.getWinDim(winW,winH);

	engine->graphics.renderText("aubrey_24","hello world!",rect2<int32>(winW / 2, 10,0,0),color(255,100,50,255));
}

void renderDebugUI(engine_state* engine, game_state* game)
{
	map_position camPos = game->cam.getCenter();
	std::string camText = "Camera pos: " + std::to_string(camPos.chunkPos.x) + " " + std::to_string(camPos.chunkPos.y) + " " + std::to_string(camPos.chunkPos.z) +
						  " " + std::to_string(camPos.realPos.x) + " " + std::to_string(camPos.realPos.y) + " " + std::to_string(camPos.realPos.z);

	engine->graphics.renderText("sans_24",camText,rect2<int32>(10,10,0,0));
}

void renderMap(engine_state* engine, game_state* game)
{
	game->cam.updateFollow();

	std::vector<rawTex*> textures;

	getMapTextures(engine,game,textures);
	sortTextures(textures);
	renderAndClearTextures(engine,textures);
}

void renderAndClearTextures(engine_state* engine, std::vector<rawTex*>& textures)
{
	// Actually render textures
	for(int32 index = 0; index < textures.size(); ++index) 
	{
		/// @note this will leave the texture with the blend mode and color mod, so be sure to set/reset it when you want to render again.
		if(rawTexture* t = dynamic_cast<rawTexture*>(textures[index]))
		{
			engine->graphics.setBlendmode(t->ID,t->blend);
			engine->graphics.setColorMod(t->ID,t->mod);
			engine->graphics.renderTextureEx(t->ID, t->pixelRect.round(), t->srcPixelRect, t->rotPoint.round(), t->rot, t->flip);
		}
		else if(rawText* t = dynamic_cast<rawText*>(textures[index]))
		{
			engine->graphics.renderTextEx(t->fontID, t->text, t->pixelRect.round(), t->srcPixelRect, t->mod, t->blend,
									      t->rotPoint.round(), t->rot, t->flip);
		}

		delete textures[index];
	}

	textures.clear();
}

void getMapTextures(engine_state* engine, game_state* game, std::vector<rawTex*>& textures)
{
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

	// Iterate through region of chunks
	for(int32 relX = TLCpos.realChunkOffset.x; relX <= BRCpos.realChunkOffset.x; relX++)
	{
		for(int32 relY = TLCpos.realChunkOffset.y; relY <= BRCpos.realChunkOffset.y; relY++)
		{
			// Get chunk
			chunk_position currentChunkPos = game->cam.getCenter().chunkPos + chunk_position(relX,relY,0);

			std::weak_ptr<chunk> currentChunk = game->map.getChunk(currentChunkPos);

			// If chunk exists
			if(!currentChunk.expired())
			{
				// Debug: draw chunk boundry
				#ifdef DRAW_CHUNK_BOUNDS
				{
					v2<real32> pixelPos = mapIntoPixelSpace( TLCpos, map_position( currentChunkPos, real_position(0,0,0) ), camZoom );
					rect2<int32> pixelChunk( std::floor(pixelPos.x), std::floor(pixelPos.y), std::ceil(CHUNK_SIZE_PIXELS * camZoom), std::ceil(CHUNK_SIZE_PIXELS * camZoom) );
					engine->graphics.renderTexture("chunkbounds", pixelChunk);
				}
				#endif

				#ifdef DRAW_CAMERA 
				{
					v2<real32> pixelPos = mapIntoPixelSpace( TLCpos, centerPos, camZoom );
					rect2<int32> pixelChunk( std::floor(pixelPos.x) - (0.5 * METERS_TO_PIXELS * camZoom), std::floor(pixelPos.y) - (0.5 * METERS_TO_PIXELS * camZoom), std::ceil(METERS_TO_PIXELS * camZoom), std::ceil(METERS_TO_PIXELS * camZoom) );
					engine->graphics.renderTexture("camera", pixelChunk);
				}
				#endif

				for(auto entry : currentChunk.lock()->entities)
				{
					std::weak_ptr<entity> e = entry.second;

					if(!e.expired() && e.lock()->hasComponent(ctype_position))
					{
						std::weak_ptr<component_position> ePosition = std::static_pointer_cast<component_position>(e.lock()->getComponent(ctype_position).lock());
						v2<real32> entityPixelPos = mapIntoPixelSpace(TLCpos,ePosition.lock()->position,camZoom);

						if(e.lock()->hasComponent(ctype_texture))
						{
							// Get component
							std::weak_ptr<component_texture> eTexture = std::static_pointer_cast<component_texture>(e.lock()->getComponent(ctype_texture).lock());
							
							// Get each texture from the entity
							for(int32 index = 0; index < eTexture.lock()->IDs.size(); index++)
							{
								textures.push_back(new rawTexture(eTexture.lock()->textures[index],camZoom,entityPixelPos));
							}
						}

						if(e.lock()->hasComponent(ctype_text_texture))
						{
							// Get component
							std::weak_ptr<component_text_texture> eText = std::static_pointer_cast<component_text_texture>(e.lock()->getComponent(ctype_text_texture).lock());
							
							// Get each texture from the entity
							for(int32 index = 0; index < eText.lock()->IDs.size(); index++)
							{
								textures.push_back(new rawText(eText.lock()->textures[index],camZoom,entityPixelPos));
							}
						}
					}
				}
			}
		}
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

void sortTextures(std::vector<rawTex*>& textures)
{
	// Sort based on y-position
	std::sort(textures.begin(), textures.end(), 
	    [](const rawTex* a, const rawTex* b) -> bool
	{ 
		if(a->layer < b->layer) return true;
		if(a->layer > b->layer) return false;
	    return (a->pixelRect.y + a->pixelRect.h) < (b->pixelRect.y + b->pixelRect.h); 
	});
}

// Terminating precompiler directives  ////////////////////////////////////////
