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

// Header files ///////////////////////////////////////////////////////////////

#include "render.h"

#include "game_state.h"

#include <algorithm>
#include <vector>
#include <string>
#include <cmath>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

void render(engine_state* engine, game_state* game)
{
	renderMap(engine,game);
	renderHUD(engine,game);
	engine->graphics.displayFrame();
}

void renderHUD(engine_state* engine, game_state* game)
{
	engine->graphics.renderText("aubrey_150","hello world!",rect2<int32>(10,10,250,125),color(255,100,50,255));
}

void renderMap(engine_state* engine, game_state* game)
{
	game->cam.updateFollow();

	std::vector<rawTexture> textures;
	std::vector<rawText> text_textures;

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
							std::weak_ptr<component_position> ePosition = std::static_pointer_cast<component_position>(e.lock()->getComponent(ctype_position).lock());
							std::weak_ptr<component_texture> eTexture = std::static_pointer_cast<component_texture>(e.lock()->getComponent(ctype_texture).lock());
							
							// Get each texture from the entity
							for(int32 texIndex = 0; texIndex < eTexture.lock()->IDs.size(); texIndex++)
							{
								// Get texture information
								v2<real32> texPos = eTexture.lock()->texturePositions[texIndex];
								v2<real32> texDim = eTexture.lock()->textureDimensions[texIndex];

								// Map the texture into pixel space (against TLC of window)
								/// @todo z-space
								v2<real32> texPixelPos = entityPixelPos + ( texPos * METERS_TO_PIXELS * camZoom);

								// Create new raw texture to render
								rawTexture texture;

								texture.pixelPos = texPixelPos;
								texture.pixelDim = texDim * METERS_TO_PIXELS * camZoom;

								texture.ID = eTexture.lock()->textureIDs[texIndex];
								texture.blend = eTexture.lock()->textureBlends[texIndex];
								texture.mod = eTexture.lock()->textureMods[texIndex];
								texture.forceTop = eTexture.lock()->forceTop[texIndex];
								texture.forceBot = eTexture.lock()->forceBot[texIndex];

								textures.push_back(texture);
							}
						}

						if(e.lock()->hasComponent(ctype_text_texture))
						{
							// Get component
							std::weak_ptr<component_text_texture> eText = std::static_pointer_cast<component_text_texture>(e.lock()->getComponent(ctype_text_texture).lock());
							
							// Get each texture from the entity
							for(int32 index = 0; index < eText.lock()->IDs.size(); index++)
							{
								// Get texture information
								std::string fontID = eText.lock()->fontIDs[index];
								std::string message = eText.lock()->text[index];
								v2<real32> texPos = eText.lock()->textPositions[index];
								v2<real32> texDim = eText.lock()->textDimensions[index];
								blendmode b = eText.lock()->textBlends[index];
								color c = eText.lock()->textMods[index];

								// Map the text into pixel space (against TLC of window)
								v2<real32> texPixelPos = entityPixelPos + ( texPos * METERS_TO_PIXELS * camZoom);

								// Create new raw text to render
								rawText rawTextTexture;
								rawTextTexture.fontID = fontID;
								rawTextTexture.text = message;
								rawTextTexture.pixelPos = texPixelPos;
								rawTextTexture.pixelDim = texDim * METERS_TO_PIXELS * camZoom;
								rawTextTexture.blend = b;
								rawTextTexture.mod = c;

								text_textures.push_back(rawTextTexture);
							}
						}
					}
				}
			}
		}
	}

	// Sort textures
	sortTextures(textures);

	// Actually render textures
	for(rawTexture& t : textures) 
	{
		/// @note this will leave the texture with the blend mode and color mod, so be sure to set/reset it when you want to render again.
		engine->graphics.setBlendmode(t.ID,t.blend);
		engine->graphics.setColorMod(t.ID,t.mod);
		engine->graphics.renderTexture(t.ID, rect2<int32>( std::round(t.pixelPos.x), std::round(t.pixelPos.y), std::round(t.pixelDim.x), std::round(t.pixelDim.y)));
	}

	for(rawText& t : text_textures) 
	{
		engine->graphics.renderText(t.fontID, t.text, rect2<int32>( std::round(t.pixelPos.x), std::round(t.pixelPos.y), std::round(t.pixelDim.x), std::round(t.pixelDim.y)),
									t.mod, t.blend);
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
		if(a.forceTop) return false;
		if(b.forceTop) return true;
		if(a.forceBot) return true;
		if(b.forceBot) return false;
	    return (a.pixelPos.y + a.pixelDim.y) < (b.pixelPos.y + b.pixelDim.y); 
	});

#if 0
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
#endif
}

// Terminating precompiler directives  ////////////////////////////////////////
