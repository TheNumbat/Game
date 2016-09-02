#pragma once

#include "game.h"

s32 threadTest(void* _g) {
	game* g = (game*)_g;
	while (g->runThreads) {
		logSetContext("TEST THREAD");
		logInfoLvl("TEST THREAD OUTPUT", 0);
		g->e->thread.delay(1000);
	}
	return 0;
}

game::game(engine* _e) {
	logSetContext("GAME");
	e = _e;

	logInfo("Intializing game...");
	logEnterSec();

	logInfo("Loading resources...");
	logEnterSec();

	logInfo("Loading textures");
	e->gfx.loadTexFolder("test/");
	e->gfx.renderTexture("test", r2<s32>(0, 0, 1280, 720));
	e->gfx.swapFrame();

	logInfo("Loading sounds");
	e->audio.loadSoundFolder("test/");
	e->audio.play("test");

	logInfo("Loading files");
	e->file.loadFile("test", "test/test.png", file_binary, file_read);

	logInfo("Spawning threads");
	runThreads = true;
	for (int i = 0; i < 5; i++)
		e->thread.add("test" + std::to_string(i), &threadTest, this);

	logInfo("Done loading resources");
	logExitSec();

	logInfo("Done initializing game.");
	logExitSec();

	running = true;
}

game::~game() {
	logSetContext("GAME");

	logInfo("Shutting down game...");
	logEnterSec();

	logInfo("Shutting down threads");
	logEnterSec();
	runThreads = false;
	int ret;
	for (int i = 0; i < 5; i++)
		e->thread.wait("test" + std::to_string(i), ret);
	logExitSec();

	logInfo("Done destroying game.");
	logExitSec();
}

bool game::run() {
	event* evt = e->input.getNext();
	if (evt->type == evt_quit) running = false;
	delete evt;
	return running;
}

void game::startReload() {
	logSetContext("RELOAD");
	logInfo("Shutting down threads.");
	runThreads = false;
	int ret;
	for (int i = 0; i < 5; i++)
		e->thread.wait("test" + std::to_string(i), ret);
}

void game::endReload() {
	logSetContext("RELOAD");
	logInfo("Spawning threads");
	runThreads = true;
	for (int i = 0; i < 5; i++)
		e->thread.add("test" + std::to_string(i), &threadTest, this);
}
