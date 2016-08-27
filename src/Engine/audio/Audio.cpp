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
	return true;
}

bool Audio::kill() {
	return true;
}

bool Audio::loadSound(const std::string& sID, const std::string& path) {
	return true;
}

bool Audio::laodSoundFolder(const std::string& path) {
	return true;
}

bool Audio::freeSound(const std::string& sID) {
	return true;
}

bool Audio::play(const std::string& sID, u32 loop, s32 timeout) {
	return true;
}

bool Audio::pause(const std::string& sID) {
	return true;
}

bool Audio::resume(const std::string& sID) {
	return true;
}

bool Audio::stop(const std::string& sID, u32 timeout) {
	return true;
}

bool Audio::pauseAll() {
	return true;
}

bool Audio::resumeAll() {
	return true;
}

bool Audio::stopAll(u32 timeout) {
	return true;
}
