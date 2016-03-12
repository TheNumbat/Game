
#ifdef MOD_EXPORTS
#define MOD_API extern "C" __declspec(dllexport)
#else
#define MOD_API __declspec(dllimport)
#endif

#include <game_state.h>
#include <engine_state.h>

MOD_API void startup(game_state* game, engine_state* engine)
{
	game->logger.LogInfo("Started up Test Mod");
}

MOD_API void update(game_state* game, engine_state* engine)
{

}

MOD_API void shutdown(game_state* game, engine_state* engine)
{
	game->logger.LogInfo("Shut down Test Mod");
}
