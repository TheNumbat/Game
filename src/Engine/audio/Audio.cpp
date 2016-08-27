#include "audio.h"

#include "..\log\log.h"

#include <SDL_mixer.h>

sound::sound() {
	sdl_mixchunk = NULL;
	channel = -1;
	good = false;
}

sound::~sound() {
	free();
}

bool sound::load(const std::string& path) {
	free();

	sdl_mixchunk = Mix_LoadWAV(path.c_str());
	assert(sdl_mixchunk);
	if (!sdl_mixchunk) {
		logWarn("Couldn't load sound from path " + path);
		return false;
	}
	good = true;
	return true;
}

bool sound::free() {
	if (good) {
		Mix_FreeChunk((Mix_Chunk*)sdl_mixchunk);
		good = false;
		return true;
	}
	return false;
}

Audio::Audio() {

}

Audio::~Audio() {

}

bool Audio::init() {

}

bool Audio::kill() {

}

bool Audio::loadSound(const std::string& sID, const std::string& path) {

}

bool Audio::laodSoundFolder(const std::string& path) {

}

bool Audio::freeSound(const std::string& sID) {

}

bool Audio::play(const std::string& sID, u32 loop, s32 timeout) {

}

bool Audio::pause(const std::string& sID) {

}

bool Audio::resume(const std::string& sID) {

}

bool Audio::stop(const std::string& sID, u32 timeout) {

}

bool Audio::pauseAll() {

}

bool Audio::resumeAll() {

}

bool Audio::stopAll(u32 timeout) {

}
