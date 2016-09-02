
#include "common.h"
#include <engine.h>
#include "game.h"

// NO GLOBALS OR STATICS

GAME_API void* startup(engine* e) {
	return new game(e);
}

GAME_API bool run(game* g) {
	return g->run();
}

GAME_API void startReload(game* g) {
	g->startReload();
}

GAME_API void endReload(game* g) {
	g->endReload();
}

GAME_API void shutdown(game* g) {
	delete g;
}
