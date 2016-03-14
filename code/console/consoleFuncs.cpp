
#include <game_state.h>
#include <engine_state.h>

#define CONSOLE_FUNC(func) extern "C" __declspec(dllexport) void func(game_state* game, engine_state* engine, const std::string& args)

#include <string>

CONSOLE_FUNC(test)
{
	game->logger.LogInfo("Console func test: " + args);
}
