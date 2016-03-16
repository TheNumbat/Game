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

/// Conversion from meters to pixels
const real32 METERS_TO_PIXELS = 64.0f;
/// Conversion from pixels to meters
const real32 PIXELS_TO_METERS = (1 / METERS_TO_PIXELS);

struct game_state;
class engine_state;

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief manages input for the game: depending on the input state events will
		   do different things
*/
struct renderMgr
{
	/**
		@brief renderMgr constructor

		Sets up log and such

		@param[in] g pointer to game state
		@param[in] e pointer to engine state
	*/
	GAME_API renderMgr(game_state* g, engine_state* e);

	/**
		@brief renderMgr constructor

		Does nothing;
	*/
	GAME_API ~renderMgr();

	/**
		@brief renders the map
	*/
	GAME_API void renderMap();

	/**
		@brief renders the debugger/profiler/console UI
	*/
	GAME_API void renderDebugHUD();
	
	/**
		@brief Describes shared information needed to render a texture
	*/
	struct rawTex
	{
		/**
			@brief rawTex destructor

			Polymorphic
		*/
		GAME_API virtual ~rawTex() = 0 {};

		/// rectangle to render into on screen
		rect2<real32> pixelRect;
		/// blend mode
		blendmode blend;
		/// color mod
		color mod;
		/// layer
		int32 layer;
		/// rectangle to pull from loaded texture
		rect2<int32> srcPixelRect;
		/// point to rotate about
		v2<real32> rotPoint;
		/// rotation in degrees
		real32 rot;
		/// flip (FLIP_NONE,FLIP_HORZ,FLIP_VERT,FLIP_HORZ|FLIP_VERT)
		uint32 flip;
	};

	/**
		@brief Describes information needed to render a normal texture
	*/
	struct rawTexture : public rawTex
	{
		/**
			@brief rawTexture constructor
		*/
		GAME_API rawTexture(const std::string& tID, const rect2<real32>& rect, blendmode b, color c, uint32 l,
				  		    const rect2<int32>& spr, const v2<real32>& rpt, real32 r, uint32 f);
		/**
			@brief rawTexture constructor using a sub_texture
		*/
		GAME_API rawTexture(const component_texture::sub_texture& t, real32 zoom, const v2<real32>& offset);
		/// ID of loaded texture to render
		std::string ID;
	};

	/**
		@brief Describes information needed to render a text texture
	*/
	struct rawText : public rawTex
	{
		/**
			@brief rawText constructor
		*/
		GAME_API rawText(const std::string& fID, const std::string& message, const rect2<real32>& rect, blendmode b, color c, uint32 l,
			    		 const rect2<int32>& spr, const v2<real32>& rpt, real32 r, uint32 f);
		/**
			@brief rawTexture constructor using a sub_text_texture
		*/
		GAME_API rawText(const component_text_texture::sub_text_texture& t, real32 zoom, const v2<real32>& offset);
		/// ID of loaded font to use
		std::string fontID;
		/// Actual text to render
		std::string text;
	};

	/**
		@brief sorts a vector of textures using their layers/y-positions

		@param textures vector to sort
	*/
	GAME_API void sortTextures(std::vector<renderMgr::rawTex*>& textures);

	/**
		@brief gets the textures from the currently visible portion of the map

		@param[out] textures vector to put textures into
	*/
	GAME_API void getMapTextures(std::vector<renderMgr::rawTex*>& textures);

	/**
		@brief renders and deletes a vector of textures

		@param[in] textures vector to render and delete
	*/
	GAME_API void renderAndClearTextures(std::vector<renderMgr::rawTex*>& textures);

	/**
		@brief renders the profiler information

		@param[in] node current node in the profiler tree
		@param[in] pos y-position on screen
		@param[in] level how deep into the tree we are

		@return how many functions were rendered
	*/
	GAME_API uint32 recursiveProfilerRender(std::weak_ptr<debugMgr::profileNode> node, uint32 pos, uint32 level = 0);
	
	/**
		@brief gets the distance between two map positions

		@param[in] origin position to use as 0,0
		@param[in] point position to get distance to
		@param[in] zoom current camera zoom

		@return vector distance in meters
	*/
	GAME_API v2<real32>& mapIntoPixelSpace(const map_position& origin, const map_position& point, real32 zoom);

	/// Pointer to game
	game_state* game;
	/// Pointer to engine
	engine_state* engine;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
