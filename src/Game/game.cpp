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

	u64 perfFreq = e->time.getPerfFreq();

	//////////////////////////////////////////////////////////////////////////////

	logInfo("Initializing game...");
	logEnterSec();

	debug.setFpsCap(60);

	//////////////////////////////////////////////////////////////////////////////

	logInfo("Loading resources...");
	logEnterSec();

	logInfo("Loading textures");
	e->gfx.loadTexFolder("test/");

	logInfo("Loading sounds");
	e->audio.loadSoundFolder("test/");
	e->audio.play("test");

	logInfo("Loading files");
	e->file.loadFile("test", "test/test.png", file_binary, file_read);

	logInfo("Done loading resources.");
	logExitSec();

	//////////////////////////////////////////////////////////////////////////////

	entity e = emgr.create();
	component epos = emgr.addC(e, ct_pos);
	component etex = emgr.addC(e, ct_tex);
	epos.pos->pos.chunk = cpos(-1, -1, 0);
	epos.pos->pos.real = rpos(20, 20, 0);
	etex.tex->ID = "test";
	etex.tex->posRect = r2<r32>(0, 0, 10, 10);
	map.addEntity(e);

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
	debug.beginFunc();

	events.handleEvents();
	ren.renderMap();
	ren.end();

	debug.endFunc();
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
