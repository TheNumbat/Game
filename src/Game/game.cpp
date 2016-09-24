#pragma once

#include "game.h"

game::game(engine* _e) 
	: events(_e, this), debug(_e, this), map(_e, this), ren(_e, this) {

	logSetContext("GAME");
	e = _e;
	e->init("Game", 640, 480);

	//////////////////////////////////////////////////////////////////////////////

	logInfo("Initializing game...");
	logEnterSec();

	debug.setFpsCap(60);

	//////////////////////////////////////////////////////////////////////////////

	logInfo("Loading resources...");
	logEnterSec();

	logInfo("Loading textures");
	e->gfx.loadTexture("test", "test.png");

	logInfo("Loading sounds");
	e->audio.loadSound("test","test.mp3");
	e->audio.play("test");

	logInfo("Loading fonts");
	e->gfx.loadFont("debug_small", "debug_assets/OpenSans.ttf", 12, font_normal);
	e->gfx.loadFont("debug_small_blue", "debug_assets/OpenSans.ttf", 12, font_normal, color(50, 100, 255, 255));

	logInfo("Loading files");
	e->file.loadFile("test", "test.png", file_binary, file_read);

	logInfo("Done loading resources.");
	logExitSec();

	debug.beginThreadProf("main", 0, true);
	
	ren.init();

	//////////////////////////////////////////////////////////////////////////////

	entity en = emgr.create();
	component epos = emgr.addC(en, ct_pos);

	epos.pos->pos.real = rpos(3, 3, 0);

	component etex = emgr.addC(en, ct_tex);
	etex.tex->ID = "test";
	etex.tex->layer = -1;
	etex.tex->posRect = r2<r32>(0, 0, 10, 10);

	etex = emgr.addC(en, ct_tex);
	etex.tex->ID = "debug_camera";
	etex.tex->posRect = r2<r32>(-.5, -.5, 1, 1);

	component etext = emgr.addC(en, ct_text);
	etext.text->font = "debug_small";
	etext.text->msg = "AYYY";
	etext.text->posRect = r2<r32>(-.5, -.5, 1, 1);

	component ephys = emgr.addC(en, ct_phys);
	ephys.phys->lastUpdate = e->time.get("physics");

	map.registerEntity(en);
	map.registerPlayer(0, en);
	ren.setFollow(en);

	debug.setFlag(renderChunkbounds);
	debug.setFlag(renderCamera);
	debug.setFlag(renderPositionText);

	logInfo("Done initializing game.");
	logExitSec();

	debug.callConsoleFunc("log CONSOLE testing callConsoleFunc");

	running = true;
}

game::~game() {
	logSetContext("GAME");

	logInfo("Shutting down game...");
	logEnterSec();

	ren.kill();

	//////////////////////////////////////////////////////////////////////////////

	logInfo("Done destroying game.");
	logExitSec();

	e->kill();
}

bool game::run() {
	debug.beginFrame();
	
	//////////////////////////////////////////////////////////////////////////////

	debug.beginFunc(0);

	events.handleEvents();

	map.runPhysics();

	ren.batchMap();
	ren.render(); // renders batch

	debug.endFunc();

	//////////////////////////////////////////////////////////////////////////////

	ren.batchDebugHUD();
	ren.render();
	e->gfx.swapFrame();

	debug.endFrame();
	return running;
}

void game::startReload() {
	logSetContext("RELOAD");
	logInfo("Shutting down threads.");
}

void game::endReload() {
	logSetContext("RELOAD");
	logInfo("Spawning threads");
	debug.reloadConsoleFuncs();
}
