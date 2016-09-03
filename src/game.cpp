#pragma once

#include "game.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>

s32 threadTest(void* _g) {
	game* g = (game*)_g;
	while (g->runThreads) {
		logSetContext("TEST THREAD");
		logInfoLvl("TEST THREAD OUTPUT", 0);
		g->thread.delay(1000);
	}
	return 0;
}

game::game() : log(this) {
	globalLog = &log;
	log.init();

	logSetContext("GAME");

	logInfo("Intializing game...");
	logEnterSec();

	logInfo("Initializing SDL");
	bool result = SDL_Init(0) == 0;
	assert(result);
	if (!result) {
		logErr((std::string) "Failed to initialize SDL! Error: " + SDL_GetError());
		logExitSec();
		return;
	}

	gfx.init("Game", 1280, 720);
	audio.init();
	time.init();
	file.init();
	input.init();
	thread.init();

	logInfo("Loading resources...");
	logEnterSec();

	logInfo("Loading textures");
	gfx.loadTexFolder("test/");
	gfx.renderTexture("test", r2<s32>(0, 0, 1280, 720));
	gfx.swapFrame();

	logInfo("Loading sounds");
	audio.loadSoundFolder("test/");
	audio.play("test");

	logInfo("Loading files");
	file.loadFile("test", "test/test.png", file_binary, file_read);

	logInfo("Spawning threads");
	runThreads = true;
	for (int i = 0; i < 5; i++)
		thread.add("test" + std::to_string(i), &threadTest, this);

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
		thread.wait("test" + std::to_string(i), ret);
	logExitSec();

	gfx.kill();
	audio.kill();
	time.kill();
	file.kill();
	input.kill();
	thread.kill();

	logInfo("Done shutting down engine.");
	log.kill();
	SDL_Quit();

	logInfo("Done destroying game.");
	logExitSec();
}

bool game::run() {
	event* evt = input.getNext();
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
		thread.wait("test" + std::to_string(i), ret);
}

void game::endReload() {
	logSetContext("RELOAD");
	logInfo("Spawning threads");
	runThreads = true;
	for (int i = 0; i < 5; i++)
		thread.add("test" + std::to_string(i), &threadTest, this);
}
