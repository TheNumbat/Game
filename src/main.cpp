
#include <engine.h>
#include <iostream>
#include <string>

typedef void* (*startupFunc)(engine*);
typedef bool (*runFunc)(void*);
typedef void(*shutdownFunc)(void*);

s32 main() {
	logSetContext("LAUNCHER");

	engine* e = new engine;

	if (!e->file.loadLib("game", "game.dll")) {
		logErr("Failed to load game library!");
		return 1;
	}
	
	startupFunc startup = (startupFunc) e->file.loadFunc("game", "startup");
	runFunc run = (runFunc) e->file.loadFunc("game", "run");
	shutdownFunc shutdown = (shutdownFunc) e->file.loadFunc("game", "shutdown");

	if (!startup || !run || !shutdown) {
		logErr("Failed to load game functions!");
		return 1;
	}

	void* game = (*startup)(e);

	while ((*run)(game));

	shutdown(game);

	e->file.freeLib("game");
	delete e;
	
	return 0;
}
