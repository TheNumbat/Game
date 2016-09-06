
#include "common.h"
#include <engine.h>
#include "game.h"

// NO GLOBALS OR STATICS

DLL_OUT void* startup(engine* e) {
	return new game(e);
}

DLL_OUT bool run(game* g) {
	return g->run();
}

DLL_OUT void startReload(game* g) {
	g->startReload();
}

DLL_OUT void endReload(game* g) {
	g->endReload();
}

DLL_OUT void shutdown(game* g) {
	delete g;
}
