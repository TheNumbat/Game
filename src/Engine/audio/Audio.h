#pragma once

#include "..\common.h"

#include <map>

class sound {
	sound();
	~sound();

	bool load(const std::string& path);
	bool free();

	void* sdl_mixchunk;
	s32 channel;
	bool good;

	friend class Audio;
};

class Audio {
public:
	ENGINE_API Audio();
	ENGINE_API ~Audio();

	ENGINE_API bool init();
	ENGINE_API bool kill();

	ENGINE_API bool loadSound(const std::string& sID, const std::string& path);
	ENGINE_API bool loadSoundFolder(const std::string& path);

	ENGINE_API bool freeSound(const std::string& sID);

	ENGINE_API bool play(const std::string& sID, u32 loop = 0, s32 timeout = -1);
	ENGINE_API bool pause(const std::string& sID);
	ENGINE_API bool resume(const std::string& sID);
	ENGINE_API bool stop(const std::string& sID, u32 timeout = 0);

	ENGINE_API bool pauseAll();
	ENGINE_API bool resumeAll();
	ENGINE_API bool stopAll(u32 timeout = 0);

private:
	std::map<std::string, sound*> sounds;
	bool good;
};

