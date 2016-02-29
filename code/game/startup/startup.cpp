// Program Information ////////////////////////////////////////////////////////

/**
	@file startup.cpp
	@author Max Slater

	@brief Sets up the game and engine, loads files, etc

	@version 1.00 (18 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

#define LOAD_FONT(path,ID,size) engine->graphics.loadFont((#path),(#ID),size)
#define LOAD_TEXTURE(path,ID) engine->graphics.loadTexture((#path),(#ID))
#define LOAD_SOUND(path,ID) engine->audio.loadSound((#path),(#ID))
#define LOAD_FILE(path,ID) engine->file.loadFile((#path),(#ID)) 

// Header files ///////////////////////////////////////////////////////////////

#include "startup.h"

#include "game_state/game_state.h"
#include <engine_state.h>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

/**
	@brief Sets up the game and engine, loads files, etc

	@param[in] engine pointer to engine state
	@param[in] game pointer to game state
*/
void gamestart(engine_state* engine, game_state* game)
{
	game->logger.StartLog("GAME");
	engine->logger.StartLog("ENGINE");

	engine->sdl.init();
	engine->graphics.init("Game");
	engine->events.init();
	engine->audio.init();
	engine->time.init();

	LOAD_TEXTURE( debug/camera.png , camera );
	LOAD_TEXTURE( debug/chunkbounds.bmp , chunkbounds );
	LOAD_TEXTURE( art/entities/dankdude_front.png , dankdude );
	LOAD_TEXTURE( art/entities/yeti.png , yeti );
	LOAD_TEXTURE( art/entities/light_circle.png , light );

	LOAD_FONT( fonts/aubrey.ttf , aubrey_24 , 24 );
	LOAD_FONT( fonts/Cenobyte.ttf , cenobyte_24 , 24 );
	LOAD_FONT( fonts/OpenSans.ttf , sans_24 , 24 );

	LOAD_SOUND( music/song1.wav , music );

	game->logger.LogInfo("Game code initialized");
	game->running = true;

	/// @todo Move this!!!
	std::weak_ptr<entity> player = game->map.addPlayer("p1",map_position(0,0,0,3,3,0),0);
	std::weak_ptr<entity> test = game->map.addEntity(map_position(0,0,0,3,3,0),0);

	std::weak_ptr<component_movement> mov = std::static_pointer_cast<component_movement>(player.lock()->addComponent(ctype_movement).lock());
	mov = std::static_pointer_cast<component_movement>(test.lock()->addComponent(ctype_movement).lock());
	mov.lock()->velocity = v2<real32>(3,0);

	std::weak_ptr<component_texture> texture = std::static_pointer_cast<component_texture>(player.lock()->addComponent(ctype_texture).lock());
	texture.lock()->addTexture("glow","light",v2<real32>(-3.5,-3.5),v2<real32>(7,7),0,blend_additive);
	texture.lock()->addTexture("main","dankdude",v2<real32>(-0.5,-0.5),v2<real32>(1,1),10);

	std::weak_ptr<component_text_texture> text = std::static_pointer_cast<component_text_texture>(player.lock()->addComponent(ctype_text_texture).lock());
	text.lock()->addText("main","cenobyte_24","ayy lmao",v2<real32>(-0.5,-0.75),v2<real32>(1,0.25),1000,blend_alpha,color(255,255,0,0));

	std::weak_ptr<component_texture> testtexture = std::static_pointer_cast<component_texture>(test.lock()->addComponent(ctype_texture).lock());
	testtexture.lock()->addTexture("main","yeti",v2<real32>(-0.5,-0.5),v2<real32>(1,1),1);

	game->cam.setFollowing(player);
}