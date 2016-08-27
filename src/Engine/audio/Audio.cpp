#include "audio.h"

#include "..\log\log.h"

#include <SDL_mixer.h>
#include <dirent.h>

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
	good = false;
}

Audio::~Audio() {
	for (auto i : sounds)
		delete i.second;
}
 
bool Audio::init() {
	logSetContext("AUDIO");
	
	logInfo("Initializing audio...");
	logEnterSec();

	if (good) {
		logWarn("Audio already initialized");
		return false;
	}

	logInfo("Opening audio stream");
	int result = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	assert(result == 0);
	if (result != 0) {
		logErr((std::string)"Failed to initalize sound, Error: " + Mix_GetError());
		return false;
	}
	
	good = true;
	logInfo("Sound initialized");
	logExitSec();
	return true;
}

bool Audio::kill() {
	logSetContext("AUDIO");

	if (!good) {
		logWarn("Audio already deinitialized");
		return false;
	}

	logInfo("Destroying audio...");
	logEnterSec();

	logInfo("Stopping all sounds");
	stopAll();

	logInfo("Quitting SDL_Mixer");
	Mix_Quit();

	good = false;
	logInfo("Done destroying audio.");
	logExitSec();
	return true;
}

bool Audio::loadSound(const std::string& sID, const std::string& path) {
	logSetContext("AUDIO");

	if(sounds.find(sID) != sounds.end())
	{
		logWarn("Sound ID " + sID + " already taken!");
		return false;
	}

	sound* newS = new sound;
	if(!newS->load(path))
	{
		logWarn("Failed to load sound ID " + sID + " from path " + path);
		return false;
	}

	sounds.insert({sID,newS});
	logInfo("Loaded sound ID " + sID + " from path " + path);
	return true;
}

bool Audio::loadSoundFolder(const std::string& path) {
	logSetContext("AUDIO");

	DIR *directory;
	dirent *entry;
	std::string dirPath = path;

	if (dirPath.back() != '/' && dirPath.back() != '\\')
	{
		dirPath.append("/");
	}

	directory = opendir(dirPath.c_str());
	assert(directory);
	if (!directory)
	{
		logWarn("Failed to open sound directory at " + dirPath);
		return false;
	}

	logInfo("Loading sound directory at " + dirPath);
	logEnterSec();

	while (entry = readdir(directory)) {
		std::string entryName = entry->d_name;

		if (entryName != ".." && entryName != ".") {
			// TODO actually test for file vs. folder 
			if (entryName[entryName.size() - 4] == '.') {
				loadSound(entryName.substr(0, entryName.length() - 4), dirPath + entryName);
			}
			else {
				loadSoundFolder(dirPath + entryName + '/');
			}
		}
	}

	logExitSec();
	return true;
}

bool Audio::freeSound(const std::string& sID) {
	logSetContext("AUDIO");

	auto entry = sounds.find(sID);
	if (entry == sounds.end()) {
		logWarn("Cannot fint to free sound ID: " + sID);
		return false;
	}

	delete entry->second;
	sounds.erase(entry);
	return true;
}

bool Audio::play(const std::string& sID, u32 loop, s32 timeout) {
	logSetContext("AUDIO");

	auto entry = sounds.find(sID);
	if (entry == sounds.end()) {
		logWarn("Could not find sound to play, ID:" + sID);
		return false;
	}

	int result = Mix_PlayChannelTimed(-1, (Mix_Chunk*)entry->second->sdl_mixchunk, loop, timeout);
	if (result == -1) {
		logWarn("Failed to play sound ID: " + sID + ", Error: " + Mix_GetError());
		return false;
	}

	entry->second->channel = result;
	return true;
}

bool Audio::pause(const std::string& sID) {
	logSetContext("AUDIO");
	
	auto entry = sounds.find(sID);
	if (entry == sounds.end()) {
		logWarn("Could not find sound to pause, ID:" + sID);
		return false;
	}

	Mix_Pause(entry->second->channel);
	return true;
}

bool Audio::resume(const std::string& sID) {
	logSetContext("AUDIO");
	
	auto entry = sounds.find(sID);
	if (entry == sounds.end()) {
		logWarn("Could not find sound to resume, ID:" + sID);
		return false;
	}

	Mix_Resume(entry->second->channel);
	return true;
}

bool Audio::stop(const std::string& sID, u32 timeout) {
	logSetContext("AUDIO");
	
	auto entry = sounds.find(sID);
	if (entry == sounds.end())
	{
		logWarn("Could not find sound to stop, ID:" + sID);
		return false;
	}

	Mix_ExpireChannel(entry->second->channel, timeout);
	return true;
}

bool Audio::pauseAll() {
	logSetContext("AUDIO");
	
	Mix_Pause(-1);
	logInfo("Paused all currently playing audio");

	return true;
}

bool Audio::resumeAll() {
	logSetContext("AUDIO");

	Mix_Resume(-1);
	logInfo("Resumed all currently paused audio");
	
	return true;
}

bool Audio::stopAll(u32 timeout) {
	logSetContext("AUDIO");
	
	Mix_ExpireChannel(-1, timeout);
	logInfo("Stopped or begun timeout of all currently playing audio");

	return true;
}
