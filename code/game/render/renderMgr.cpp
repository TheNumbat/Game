// Program Information ////////////////////////////////////////////////////////

/**
	@file renderMgr.cpp

	@brief Implementation file for the renderMgr class.

	@author Max Slater

	@version 1.00 (6 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "renderMgr.h"

#include "game_state.h"
#include <engine_state.h>

#include <algorithm>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

renderMgr::renderMgr(game_state* g, engine_state* e)
{
	game = g;
	engine = e;
}

renderMgr::~renderMgr()
{

}

renderMgr::rawTexture::rawTexture(const std::string& tID, const rect2<real32>& rect, blendmode b, color c, uint32 l,
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

renderMgr::rawTexture::rawTexture(const component_texture::sub_texture& t, real32 zoom, const v2<real32>& offset)
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

renderMgr::rawText::rawText(const std::string& fID, const std::string& message, const rect2<real32>& rect, blendmode b, color c, uint32 l,
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

renderMgr::rawText::rawText(const component_text_texture::sub_text_texture& t, real32 zoom, const v2<real32>& offset)
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

void renderMgr::renderMap()
{
	game->debug.beginProfiledFunc();

	game->camera.updateFollow();

	std::vector<renderMgr::rawTex*> textures;

	getMapTextures(textures);
	sortTextures(textures);
	renderAndClearTextures(textures);

	game->debug.endProfiledFunc();
}

void renderMgr::renderDebugUI()
{
	real64 avgFrame = 1000.0f * (real64)game->debug.getAvgFrame() / (real64)engine->time.getPerfFreq();
	real64 lastFrame = 1000.0f * (real64)game->debug.getLastFrame() / (real64)engine->time.getPerfFreq();
	std::string msg1 = "Average frame time (ms): " + std::to_string(avgFrame);
	std::string msg2 = "Last frame time (ms): " + std::to_string(lastFrame);

	engine->graphics.renderText("debugUI",msg1,rect2<int32>(10,10,0,0));
	engine->graphics.renderText("debugUI",msg2,rect2<int32>(10,32,0,0));

	recursiveProfilerRender(game->debug.profileHead,52);
}

uint32 renderMgr::recursiveProfilerRender(std::weak_ptr<debugMgr::profileNode> node, uint32 pos, uint32 level)
{
	if(node.expired())
	{
		return 0;
	}

	std::string msg;
	for(uint32 i = 0; i < level; i++)
	{
		msg += "   ";
	}
	msg = msg + node.lock()->funcName + " - self: " + std::to_string(node.lock()->self) + 
		  " heir: " + std::to_string(node.lock()->heir) + " calls: " + std::to_string(node.lock()->calls);
	engine->graphics.renderText("debugUI",msg,rect2<int32>(10,pos,0,0));

	int numchildren = 0;
	for(auto entry : node.lock()->children)
	{
		numchildren++;
		numchildren += recursiveProfilerRender(entry.second,pos + numchildren * 22, level + 1);
	}

	return numchildren;
}

void renderMgr::renderAndClearTextures(std::vector<renderMgr::rawTex*>& textures)
{
	game->debug.beginProfiledFunc();

	// Actually render textures
	for(int32 index = 0; index < textures.size(); ++index) 
	{
		/// @note this will leave the texture with the blend mode and color mod, so be sure to set/reset it when you want to render again.
		if(renderMgr::rawTexture* t = dynamic_cast<renderMgr::rawTexture*>(textures[index]))
		{
			engine->graphics.setBlendmode(t->ID,t->blend);
			engine->graphics.setColorMod(t->ID,t->mod);
			engine->graphics.renderTextureEx(t->ID, t->pixelRect.round(), t->srcPixelRect, t->rotPoint.round(), t->rot, t->flip);
		}
		else if(renderMgr::rawText* t = dynamic_cast<renderMgr::rawText*>(textures[index]))
		{
			engine->graphics.renderTextEx(t->fontID, t->text, t->pixelRect.round(), t->srcPixelRect, t->mod, t->blend,
									      t->rotPoint.round(), t->rot, t->flip);
		}
		else
		{
			game->logger.LogWarn("rip");
		}

		delete textures[index];
	}

	textures.clear();

	game->debug.endProfiledFunc();
}

void renderMgr::getMapTextures(std::vector<renderMgr::rawTex*>& textures)
{
	game->debug.beginProfiledFunc();

	// Get window size
	int32 winW, winH;
	engine->graphics.getWinDim(winW,winH);

	// Set up varaibles describing render region
	real32 camZoom = game->camera.zoom;
	real32 halfScreenWidthMeters = (winW / 2.0f) * (PIXELS_TO_METERS / camZoom);
	real32 halfScreenHeightMeters = (winH / 2.0f) * (PIXELS_TO_METERS / camZoom);

	// Camrea position is the center of the screen, so this gives us a position at the TLC to render from
	map_position centerPos = game->camera.getCenter();
	map_position offset = map_position(0,0,0,halfScreenWidthMeters,halfScreenHeightMeters,0);
	map_position TLCpos = centerPos - offset;
	map_position BRCpos = centerPos + offset;

	// Iterate through region of chunks
	for(int32 relX = TLCpos.realChunkOffset.x; relX <= BRCpos.realChunkOffset.x; relX++)
	{
		for(int32 relY = TLCpos.realChunkOffset.y; relY <= BRCpos.realChunkOffset.y; relY++)
		{
			// Get chunk
			chunk_position currentChunkPos = game->camera.getCenter().chunkPos + chunk_position(relX,relY,0);

			std::weak_ptr<chunk> currentChunk = game->map.getChunk(currentChunkPos);

			// If chunk exists
			if(!currentChunk.expired())
			{
				// Debug: draw chunk boundry
				#define DRAW_CHUNK_BOUNDS
				#ifdef DRAW_CHUNK_BOUNDS
				{
					v2<real32> pixelPos = mapIntoPixelSpace( TLCpos, map_position( currentChunkPos, real_position(0,0,0) ), camZoom );
					rect2<real32> pixelChunk( std::floor(pixelPos.x), std::floor(pixelPos.y), std::ceil(CHUNK_SIZE_METERS * METERS_TO_PIXELS * camZoom), std::ceil(CHUNK_SIZE_METERS * METERS_TO_PIXELS * camZoom) );

					textures.push_back(new rawTexture("chunkbounds",pixelChunk,blend_none,color(255,255,255,0),-1000,
													  rect2<int32>(0,0,0,0),v2<real32>(0,0),0,0));
				}
				#endif

				#ifdef DRAW_CAMERA 
				{
					v2<real32> pixelPos = mapIntoPixelSpace( TLCpos, centerPos, camZoom );
					rect2<real32> pixelChunk( std::round(pixelPos.x) - (0.5 * METERS_TO_PIXELS * camZoom), std::round(pixelPos.y) - (0.5 * METERS_TO_PIXELS * camZoom), std::round(METERS_TO_PIXELS * camZoom), std::round(METERS_TO_PIXELS * camZoom) );
					textures.push_back(new rawTexture("camera",pixelChunk,blend_alpha,color(255,255,255,0),-1,
									   rect2<int32>(0,0,0,0),v2<real32>(0,0),0,0));
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

	game->debug.endProfiledFunc();
}

v2<real32>& renderMgr::mapIntoPixelSpace(const map_position& origin, const map_position& point, real32 zoom)
{
	game->debug.beginProfiledFunc();

	// Calculate relative chunk and within-chunk positions
	v2<real32> relDistance( point.realPos.x - origin.realPos.x , point.realPos.y - origin.realPos.y );
	v2<real32> chunkDistance( point.chunkPos.x - origin.chunkPos.x , point.chunkPos.y - origin.chunkPos.y );

	// Calculate total distance
	v2<real32> totalDistanceMeters = ( chunkDistance * CHUNK_SIZE_METERS ) + relDistance;
	v2<real32> totalDistancePixels = totalDistanceMeters * METERS_TO_PIXELS * zoom;

	game->debug.endProfiledFunc();
	return totalDistancePixels;
}

void renderMgr::sortTextures(std::vector<renderMgr::rawTex*>& textures)
{
	game->debug.beginProfiledFunc();

	// Sort based on y-position
	std::sort(textures.begin(), textures.end(), 
	    [](const renderMgr::rawTex* a, const renderMgr::rawTex* b) -> bool
	{ 
		if(a->layer < b->layer) return true;
		if(a->layer > b->layer) return false;
	    return (a->pixelRect.y + a->pixelRect.h) < (b->pixelRect.y + b->pixelRect.h); 
	});

	game->debug.endProfiledFunc();
}

// Terminating precompiler directives  ////////////////////////////////////////
