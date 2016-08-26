
#include "common.h"

#include "engine.h"

#include <SDL.h>

engine::engine()
{
	SDL_Init(SDL_INIT_EVERYTHING);
}


engine::~engine()
{
	SDL_Quit();
}
