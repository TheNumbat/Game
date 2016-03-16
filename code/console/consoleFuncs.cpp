
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

CONSOLE_FUNC(addEntity)
{	
	std::stringstream strStream(args);
	int cx, cy, cz;
	real32 x, y, z;
	std::string timer;
	strStream >> cx >> cy >> cz >> x >> y >> z >> timer;
	if(!strStream.good())
	{
		game->logger.LogWarn("Error parsing input args");
		return false;
	}

	map_position pos(cx,cy,cz,x,y,z);
	game->map.addEntity(pos,timer);
	return true;	
}

CONSOLE_FUNC(addPlayer)
{
	std::stringstream strStream(args);
	int cx, cy, cz;
	real32 x, y, z;
	std::string timer, ID;
	strStream >> ID >> cx >> cy >> cz >> x >> y >> z >> timer;
	if(!strStream.good())
	{
		game->logger.LogWarn("Error parsing input args");
		return false;
	}

	map_position pos(cx,cy,cz,x,y,z);
	game->map.addPlayer(ID,pos,timer);
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

CONSOLE_FUNC(drawChunkbounds)
{
	game->debug.toggleDebugOption(renderChunkbounds);
	return true;
}

CONSOLE_FUNC(drawCamera)
{
	game->debug.toggleDebugOption(renderCamera);
	return true;
}

CONSOLE_FUNC(drawPositionText)
{
	game->debug.toggleDebugOption(renderPositionText);
	return true;
}

CONSOLE_FUNC(toggleTimer)
{
	std::stringstream strStream(args);
	std::string ID;
	strStream >> ID;
	if(!strStream.good())
	{
		game->logger.LogWarn("Error parsing input args");
		return false;
	}
	engine->time.toggle(ID);
	return true;
}

CONSOLE_FUNC(loadAsset)
{
	std::stringstream strStream(args);
	std::string type, path, ID;
	strStream >> type >> path >> ID;
	if(!strStream.good())
	{
		game->logger.LogWarn("Error parsing input args");
		return false;
	}

	if(type == "texture")
	{
		LOAD_TEXTURE(path,ID);
	}
	else if(type == "sound")
	{
		LOAD_SOUND(path,ID);
	}
	else if(type == "file")
	{
		uint8 type;
		std::string access;
		strStream >> type >> access;
		if(!strStream.good())
		{
			game->logger.LogWarn("Error parsing input args");
			return false;
		}

		LOAD_FILE(path,type,access,ID);
	}
	else if(type == "font")
	{
		int size;
		strStream >> size;
		if(!strStream.good())
		{
			game->logger.LogWarn("Error parsing input args");
			return false;
		}

		LOAD_FONT(path,ID,size);
	}
	else if(type == "library")
	{
		LOAD_LIB(path,ID);
	}
}

CONSOLE_FUNC(freeAsset)
{
	std::stringstream strStream(args);
	std::string type, ID;
	strStream >> type >> ID;
	if(!strStream.good())
	{
		game->logger.LogWarn("Error parsing input args");
		return false;
	}

	if(type == "texture")
	{
		FREE_TEXTURE(ID);
	}
	else if(type == "sound")
	{
		FREE_SOUND(ID);
	}
	else if(type == "file")
	{
		FREE_FILE(ID);
	}
	else if(type == "font")
	{
		FREE_FONT(ID);
	}
	else if(type == "library")
	{
		FREE_LIB(ID);
	}
}
