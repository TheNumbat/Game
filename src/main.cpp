
#include <iostream>
#include <string>
#include <assert.h>
#include <windows.h>

bool load();
void unload();
bool reload(void* g);

typedef void* (*startupFunc)();
typedef bool (*runFunc)(void*);
typedef void (*shutdownFunc)(void*);
typedef void (*startReloadFunc)(void*);
typedef void (*endReloadFunc)(void*);

void* lib;

startupFunc startup;
runFunc run;
shutdownFunc shut_down;
startReloadFunc startReload;
endReloadFunc endReload;

int main() {
	if ( !load() ) {
		assert(false);
	}

	void* game = (*startup)();

	while ((*run)(game)) {
		if( !reload(game) ) assert(false);
	}

	(*shut_down)(game);
	unload();
	return 0;
}

bool reload(void* g) {
	WIN32_FILE_ATTRIBUTE_DATA FData;
	int result = GetFileAttributesEx("../Game/Debug/Game.dll", GetFileExInfoStandard, &FData);
	if (!result) {
		return false;
	}

	static FILETIME LastWriteTime = FData.ftLastWriteTime;
	if (CompareFileTime(&FData.ftLastWriteTime, &LastWriteTime) == 1) {
		(*startReload)(g);
		unload();
		load();
		(*endReload)(g);

		// TODO: find out why this gets changed since the first time it's read
		GetFileAttributesEx("../Game/Debug/Game.dll", GetFileExInfoStandard, &FData);
		LastWriteTime = FData.ftLastWriteTime;
	}
	return true;
}

bool load() {
	// TODO: fix infinite loop
	// TODO: fix path for standalone build
	while ( !CopyFileA( "../Game/Debug/Game.dll", "gameTemp.dll", false )) Sleep(10);

	char buf[MAX_PATH];
	GetCurrentDirectory(sizeof(buf), buf);

	lib = LoadLibrary("gameTemp.dll");
	if (!lib) {
		return false;
	}

	startup = (startupFunc) GetProcAddress((HMODULE) lib, "startup");
	run = (runFunc) GetProcAddress((HMODULE) lib, "run");
	shut_down = (shutdownFunc) GetProcAddress((HMODULE) lib, "shutdown");
	startReload = (startReloadFunc) GetProcAddress((HMODULE) lib, "startReload");
	endReload = (endReloadFunc) GetProcAddress((HMODULE) lib, "endReload");

	if (!startup || !run || !shut_down || !startReload || !endReload) {
		unload();
		return false;
	}
	return true;
}

void unload() {
	FreeLibrary((HMODULE) lib);
	startup = NULL;
	run = NULL;
	shut_down = NULL;
	startReload = NULL;
	endReload = NULL;
}