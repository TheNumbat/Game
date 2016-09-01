
#include <engine.h>
#include <iostream>
#include <string>

bool load(engine* e);
void unload(engine* e);
bool reload(engine* e);

typedef void* (*startupFunc)(engine*);
typedef bool (*runFunc)(void*);
typedef void (*shutdownFunc)(void*);
typedef void (*startReloadFunc)(void*);
typedef void (*endReloadFunc)(void*);

startupFunc startup;
runFunc run;
shutdownFunc shut_down;
startReloadFunc startReload;
endReloadFunc endReload;

s32 main() {
	logSetContext("LAUNCHER");

	engine* e = new engine;

	if ( !load(e) ) {
		return 1;
	}

	void* game = (*startup)(e);

	while ((*run)(game)) {
		if( !reload(e) ) return 1;
	}

	shut_down(game);
	unload(e);
	delete e;
	return 0;
}

// TODO: remove windows
#include <windows.h>
bool reload(engine* e) {
	logSetContext("LAUNCHER");

	WIN32_FILE_ATTRIBUTE_DATA FData;
	int result = GetFileAttributesEx("../Game/Debug/Game.dll", GetFileExInfoStandard, &FData);
	assert(result);
	if (!result) {
		logErr("Failed to get dll file attributes!");
		return false;
	}

	static FILETIME LastWriteTime = FData.ftLastWriteTime;
	if (CompareFileTime(&FData.ftLastWriteTime, &LastWriteTime) == 1) {
		logInfo("STARTING GAME RELOAD");
		(*startReload)(e);
		unload(e);
		load(e);
		(*endReload)(e);
		logInfo("FINISHED GAME RELOAD");
	}
	return true;
}

bool load(engine* e) {
	logSetContext("LAUNCHER");

	// TODO: fix infinite loop
	// TODO: fix path for standalone build
	while (!e->file.copyFile("../Game/Debug/Game.dll", "gameTemp.dll"));

	if (!e->file.loadLib("game", "gameTemp.dll")) {
		logErr("Failed to load game library!");
		return false;
	}

	startup = (startupFunc) e->file.loadFunc("game", "startup");
	run = (runFunc) e->file.loadFunc("game", "run");
	shut_down = (shutdownFunc) e->file.loadFunc("game", "shutdown");
	startReload = (startReloadFunc) e->file.loadFunc("game", "startReload");
	endReload = (endReloadFunc) e->file.loadFunc("game", "endReload");

	if (!startup || !run || !shut_down || !startReload || !endReload) {
		unload(e);
		return false;
	}
	return true;
}

void unload(engine* e) {
	e->file.freeLib("game");
	startup = NULL;
	run = NULL;
	shut_down = NULL;
	startReload = NULL;
	endReload = NULL;
}