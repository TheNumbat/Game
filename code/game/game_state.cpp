// Program Information ////////////////////////////////////////////////////////

/**
	@file game_state.cpp
	@author Max Slater

	@brief Describes the game state

	@version 2.00 (3 Mar 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

#include "game_state.h"

#include <engine_state.h>

// Typedefs ///////////////////////////////////////////////////////////////////

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

game_state::game_state(engine_state* e)
	: map(this,e,"sim"), input(this,e), render(this,e), debug(this,e),
      mods(this,e)
{
	engine = e;
	startup();
}

game_state::~game_state()
{
	shutdown();
}

bool game_state::gameLoop()
{
	debug.beginDebugFrame();
	debug.beginProfiledFunc();

	input.handleEvents();
	map.update();
	mods.updateMods();
	render.renderMap();

	debug.endProfiledFunc();

	render.renderDebugHUD();
	engine->graphics.displayFrame();

	debug.endDebugFrame();

	return running;
}

void game_state::startup()
{
	logger.StartLog("GAME");
	engine->logger.StartLog("ENGINE");

	// Initialize engine
	engine->sdl.init();
	engine->graphics.init("Game",640,480);
	engine->events.init();
	engine->audio.init();
	engine->time.init();

	// Initialize debug
	debug.setFPSCap(60);
	debug.loadConsoleFuncs();

	// Load Media
	LOAD_TEXTURE( "debug/camera.png" , "camera" );
	LOAD_TEXTURE( "debug/chunkbounds.bmp" , "chunkbounds" );
	LOAD_TEXTURE( "art/entities/dankdude_front.png" , "dankdude" );
	LOAD_TEXTURE( "art/entities/yeti.png" , "yeti" );
	LOAD_TEXTURE( "art/entities/light_circle.png" , "light" );
	LOAD_TEXTURE( "debug/chunkbounds.bmp" , "debug_rect");
	LOAD_TEXTURE( "hh/test2/tree00.bmp" , "tree1");

	LOAD_FONT( "fonts/aubrey.ttf" , "aubrey_24" , 24 );
	LOAD_FONT( "fonts/Cenobyte.ttf" , "cenobyte_24" , 24 );
	LOAD_FONT( "fonts/OpenSans.ttf" , "debugUI" , 24 );
	LOAD_FONT( "fonts/OpenSans.ttf" , "debugUI_small" , 16 );
	LOAD_FONT( "fonts/OpenSans.ttf" , "debugUI_verysmall" , 12 );

	LOAD_SOUND( "music/song1.wav" , "music" );

	// Collision rules
	map.setCollisionRule(collision_player,collision_enviroment,true);

	// Done
	logger.LogInfo("Game code initialized");
	running = true;

	// Start mods
	mods.startup();

///////////////////////////////////////////////////////////////////////////////////////////////////

	debug.setDebugOption(renderCollisionBounds);
	debug.setDebugOption(renderChunkbounds);

	/// @todo Move this!!!
	std::weak_ptr<entity> player = map.addPlayer("p1",map_position(0,0,0,3,3,0),0);

	std::weak_ptr<component_movement> mov = std::static_pointer_cast<component_movement>(player.lock()->addComponent(ctype_movement).lock());

	std::weak_ptr<component_texture> texture = std::static_pointer_cast<component_texture>(player.lock()->addComponent(ctype_texture).lock());
	texture.lock()->addTexture("glow","light",rect2<real32>(-3.5,-3.5,7,7),rect2<int32>(0,0,0,0),1,blend_additive);
	texture.lock()->addTexture("main","dankdude",rect2<real32>(-0.5,-0.5,1,1),rect2<int32>(0,0,0,0),10,blend_alpha,color(255,255,255,0),v2<real32>(0.5,0.5));

	std::weak_ptr<component_text_texture> text = std::static_pointer_cast<component_text_texture>(player.lock()->addComponent(ctype_text_texture).lock());
	text.lock()->addText("main","cenobyte_24","ayy lmao",rect2<real32>(-0.5,-0.75,0.8,0.25),rect2<int32>(0,0,0,0),1000,blend_alpha,color(255,255,0,0));

	std::weak_ptr<component_collision> collision = std::static_pointer_cast<component_collision>(player.lock()->addComponent(ctype_collision).lock());
	collision.lock()->addRect("player",rect2<real32>(-0.5,-0.5,1,1));


	std::weak_ptr<entity> test = map.addEntity(map_position(0,0,0,5,3,0),0);
	std::weak_ptr<component_texture> testtexture = std::static_pointer_cast<component_texture>(test.lock()->addComponent(ctype_texture).lock());
	testtexture.lock()->addTexture("main","tree1",rect2<real32>(-0.5,-0.5,0.75,1),rect2<int32>(0,0,0,0),2);

	// mov = std::static_pointer_cast<component_movement>(test.lock()->addComponent(ctype_movement).lock());
	// mov.lock()->velocity = v2<real32>(3,0);

	collision = std::static_pointer_cast<component_collision>(test.lock()->addComponent(ctype_collision).lock());
	collision.lock()->addRect("test",rect2<real32>(-0.5,-0.5,1,1));

	camera.setFollowing(player);
}

void game_state::shutdown()
{
	mods.shutdown();
}

// Free function prototypes  //////////////////////////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
