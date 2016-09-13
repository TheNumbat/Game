#pragma once

#include "game.h"

s32 threadTest(void* _g) {
	game* g = (game*) _g;
	while (g->runThreads) {
		logSetContext("TEST THREAD");
		logInfoLvl("TEST THREAD OUTPUT", 0);
		g->e->thread.delay(1000);
	}
	return 0;
}

game::game(engine* _e) 
	: events(_e, this), debug(_e, this), map(_e, this), ren(_e, this) {

	logSetContext("GAME");
	e = _e;
	e->init("Game", 1280, 720);
	ren.init();

	u64 perfFreq = e->time.getPerfFreq();

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

	logInfo("Loading files");
	e->file.loadFile("test", "test.png", file_binary, file_read);

	logInfo("Done loading resources.");
	logExitSec();

	//////////////////////////////////////////////////////////////////////////////

	entity e = emgr.create();
	component epos = emgr.addC(e, ct_pos);

	epos.pos->pos.real = rpos(3, 3, 0);

	component etex = emgr.addC(e, ct_tex);
	etex.tex->ID = "test";
	etex.tex->posRect = r2<r32>(0, 0, 10, 10);

	etex = emgr.addC(e, ct_tex);
	etex.tex->ID = "debug_camera";
	etex.tex->posRect = r2<r32>(-.5, -.5, 1, 1);

	component etext = emgr.addC(e, ct_text);
	etext.text->font = "ayy";
	etext.text->msg = "lmao";
	etext.text->posRect = r2<r32>(-.5, -.5, 1, 1);

	map.addEntity(e);

	debug.setFlag(renderChunkbounds);
	debug.setFlag(renderCamera);

	logInfo("Done initializing game.");
	logExitSec();

	debug.callConsoleFunc("log CONSOLE testing callConsoleFunc");

	running = true;
}

game::~game() {
	logSetContext("GAME");

	logInfo("Shutting down game...");
	logEnterSec();

	//////////////////////////////////////////////////////////////////////////////

	logInfo("Done destroying game.");
	logExitSec();

	e->kill();
}

bool game::run() {
	debug.beginFrame();
	
	//////////////////////////////////////////////////////////////////////////////

	debug.beginFunc();

	events.handleEvents();

	ren.renderMap();
	ren.endBatch();

	debug.endFunc();

	//////////////////////////////////////////////////////////////////////////////

	ren.renderDebugHUD();
	ren.endBatch();
	e->gfx.swapFrame();

	debug.endFrame();
	return running;
}

void game::startReload() {
	logSetContext("RELOAD");
	logInfo("Shutting down threads.");
	ren.stopThread();
}

void game::endReload() {
	logSetContext("RELOAD");
	logInfo("Spawning threads");
	debug.reloadConsoleFuncs();
	ren.startThread();
}
