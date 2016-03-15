
#include <game_state.h>
#include <engine_state.h>

#define CONSOLE_FUNC(func) extern "C" __declspec(dllexport) bool func(game_state* game, engine_state* engine, const std::string& args)

#include <string>
#include <sstream>

CONSOLE_FUNC(quit)
{
	game->running = false;
	return true;
}

CONSOLE_FUNC(print)
{
	game->logger.LogInfo("Console: " + args);
	return true;
}

CONSOLE_FUNC(setPlayerPos)
{
	std::stringstream strStream(args);
	int x, y, z, cx, cy, cz;
	std::string playerID;
	strStream >> playerID >> x >> y >> z >> cx >> cy >> cz;
	if(!strStream.good())
	{
		game->logger.LogWarn("Error parsing input args");
		return false;
	}
	std::weak_ptr<entity> player = game->map.getPlayerByID(playerID);
	std::weak_ptr<component_position> pos = std::static_pointer_cast<component_position>(player.lock()->getComponent(ctype_position).lock());
	pos.lock()->position = map_position(x,y,z,cx,cy,cz);
	pos.lock()->position.realChunkOffset = chunk_position(cx,pos.lock()->position.chunkPos.y - cy,pos.lock()->position.chunkPos.z - cz);
	game->map.updateEntityMapPos(player);
	return true;
}

CONSOLE_FUNC(setZoom)
{
	std::stringstream strStream(args);
	float zoom;
	strStream >> zoom;
	if(!strStream.good())
	{
		game->logger.LogWarn("Error parsing input args");
		return false;
	}
	game->camera.zoom = zoom;
	return true;
}

CONSOLE_FUNC(toggleDrawChunkbounds)
{
	game->debug.toggleDebugOption(renderChunkbounds);
	return true;
}

CONSOLE_FUNC(toggleDrawCamera)
{
	game->debug.toggleDebugOption(renderCamera);
	return true;
}
