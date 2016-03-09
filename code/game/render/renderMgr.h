// Program Information ////////////////////////////////////////////////////////

/**
	@file renderMgr.h

	@brief Declartion file for the renderMgr class.

	See renderMgr.cpp for member function documentation

	@author Max Slater

	@version 1.00 (6 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "game_common.h"

#include "map\position\position.h"
#include "map\component\component.h"
#include "debug\debugMgr.h"

#include <vector>
#include <vect.h>
#include <rect.h>

// Global constant definitions  ///////////////////////////////////////////////

const real32 METERS_TO_PIXELS = 64.0f;
const real32 PIXELS_TO_METERS = (1 / METERS_TO_PIXELS);

class game_state;
class engine_state;

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief manages input for the game: depending on the input state events will
		   do different things
*/
class renderMgr
{
public:
	/**
		@brief renderMgr constructor

		Sets up log and such

		@param[in] g pointer to game state
		@param[in] e pointer to engine state
	*/
	renderMgr(game_state* g, engine_state* e);

	/**
		@brief renderMgr constructor

		Does nothing;
	*/
	~renderMgr();

	void renderMap();
	void renderDebugUI();
	void toggleDebugUI();

private:
	class rawTex
	{
	protected:
		virtual ~rawTex() = 0 {};
		rect2<real32> pixelRect;
		blendmode blend;
		color mod;
		int32 layer;
		rect2<int32> srcPixelRect;
		v2<real32> rotPoint;
		real32 rot;
		uint32 flip;
		friend class renderMgr;
	};

	class rawTexture : public rawTex
	{
		rawTexture(const std::string& tID, const rect2<real32>& rect, blendmode b, color c, uint32 l,
				   const rect2<int32>& spr, const v2<real32>& rpt, real32 r, uint32 f);
		rawTexture(const component_texture::sub_texture& t, real32 zoom, const v2<real32>& offset);
		std::string ID;
		friend class renderMgr;
	};

	class rawText : public rawTex
	{
		rawText(const std::string& fID, const std::string& message, const rect2<real32>& rect, blendmode b, color c, uint32 l,
			    const rect2<int32>& spr, const v2<real32>& rpt, real32 r, uint32 f);
		rawText(const component_text_texture::sub_text_texture& t, real32 zoom, const v2<real32>& offset);
		std::string fontID;
		std::string text;
		friend class renderMgr;
	};


	void sortTextures(std::vector<renderMgr::rawTex*>& textures);
	void getMapTextures(std::vector<renderMgr::rawTex*>& textures);
	void renderAndClearTextures(std::vector<renderMgr::rawTex*>& textures);
	uint32 recursiveProfilerRender(std::weak_ptr<debugMgr::profileNode> node, uint32 pos, uint32 level = 0);
	
	v2<real32>& mapIntoPixelSpace(const map_position& origin, const map_position& point, real32 zoom);

	bool showDebugUI;

	game_state* game;
	engine_state* engine;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
