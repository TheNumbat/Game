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
	Audio();
	~Audio();

	bool init();
	bool kill();

	bool loadSound(const std::string& sID, const std::string& path);
	bool loadSoundFolder(const std::string& path);

	bool freeSound(const std::string& sID);

	bool play(const std::string& sID, u32 loop = 0, s32 timeout = -1);
	bool pause(const std::string& sID);
	bool resume(const std::string& sID);
	bool stop(const std::string& sID, u32 timeout = 0);

	bool pauseAll();
	bool resumeAll();
	bool stopAll(u32 timeout = 0);

private:
	// TODO: region allocation, free list
	std::map<std::string, sound*> sounds;
	bool good;
};

