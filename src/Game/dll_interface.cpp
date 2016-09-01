
#include "common.h"
#include <engine.h>
#include "game.h"

bool runThreads = true;
s32 threadTest(void* data) {
	engine* e = (engine*)data;
	while (runThreads) {
		logSetContext("TEST THREAD");
		logInfoLvl("TEST THREAD OUTPUT", 0);
		e->thread.delay(1000);
	}
	return 0;
}


GAME_API void* startup(engine* e) {
	logSetContext("GAME");
	logInfo("Initializing game...");
	logEnterSec();

	game* g = new game(e);

	logInfo("Loading stuff");
	e->gfx.loadTexFolder("test/");
	e->gfx.renderTexture("test", r2<s32>(0, 0, 1280, 720));
	e->gfx.swapFrame();

	e->audio.loadSoundFolder("test/");
	e->audio.play("test");

	e->file.loadFile("test", "test/test.png", file_binary, file_read);

	logInfo("Spawning threads");
	for (int i = 0; i < 5; i++)
		e->thread.add("test" + std::to_string(i), &threadTest, e);

	logInfo("Done initializing game.");
	logExitSec();

	g->running = true;

	return g;
}

GAME_API bool run(game* g) {
	event* evt = g->e->input.getNext();
	if (evt->type == evt_quit) g->running = false;
	delete evt;
	return g->running;
}

GAME_API void startReload(game* g) {
	logSetContext("RELOAD");
	logInfo("Shutting down threads.");
	runThreads = false;
	int ret;
	for (int i = 0; i < 5; i++)
		g->e->thread.wait("test" + std::to_string(i), ret);
}

GAME_API void endReload(game* g) {
	logSetContext("RELOAD");
	logInfo("Spawning threads");
	for (int i = 0; i < 5; i++)
		g->e->thread.add("test" + std::to_string(i), &threadTest, g->e);
}

GAME_API void shutdown(game* g) {
	logSetContext("GAME");

	logInfo("Destroying game...");
	logEnterSec();

	logInfo("Shutting down threads.");
	runThreads = false;
	int ret;
	for (int i = 0; i < 5; i++)
		g->e->thread.wait("test" + std::to_string(i), ret);

	delete g;
	logInfo("Done destroying game.");
	logExitSec();
}
