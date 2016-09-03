
#include "common.h"
#include "game.h"

// NO GLOBALS OR STATICS

DLL_API void* startup() {
	return new game();
}

DLL_API bool run(game* g) {
	return g->run();
}

DLL_API void startReload(game* g) {
	g->startReload();
}

DLL_API void endReload(game* g) {
	g->endReload();
}

DLL_API void shutdown(game* g) {
	delete g;
}
