// Program Information ////////////////////////////////////////////////////////

/**
	@file soundMgr.h

	@brief Declartion file for soundMgr class

	Manges the outout of sound, simalarly to how graphicMgr manages the output
	of textures.

	@author Max Slater

	@version 1.00 (10 Feb 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include "log/logMgr.h"
#include "sound/sound.h"

#include <string>
#include <memory>
#include <map>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Manges the outout of sound, simalarly to how graphicMgr manages the output
	of textures.
*/
class soundMgr
{
public:
	/**
		@brief soundMgr constructor

		Defaults values and starts log
		Call init() to set up sound
	*/
	ENGINE_API soundMgr();

	/**
		@brief soundMgr denstructor

		Calls kill()
	*/
	ENGINE_API ~soundMgr();

	/**
		@brief initializes sound

		Calls Mix_OpenAdio

		@return success

		@exception already initialized, returns false
		@exception failed to open audio, returns false
	*/
	ENGINE_API bool init();

	/**
		@brief deinitializes sound

		Calls Mix_Quit

		@return success

		@exception sound already deinitialized, returns false
	*/
	ENGINE_API bool kill();

	/**
		@brief Loads a sound from a file

		@param[in] path to sound file to load
		@param[in] ID of sound

		@return success

		@exception failed to load sound, returns false
	*/
	ENGINE_API bool loadSound(const std::string& path, const std::string& ID = "");

	/**
		@brief Loads all image files from a folder (and subfolders) into textute map

		Will just use file name for texture IDs

		@param[in] path file path of image file

		@note uses dirent.h

		@return success -- always true
	*/
	ENGINE_API bool loadSoundRec(const std::string& path);

	/**
		@brief unloads a sound

		@param[in] ID of sound to unload

		@return sucess

		@exception sound not found, returns false
	*/
	ENGINE_API bool freeSound(const std::string& ID);

	/**
		@brief Pauses all currently playing sounds

		@return success -- always true
	*/
	ENGINE_API bool pauseAll();

	/**
		@brief Resumes all currently paused or playing channels

		@return success -- always true
	*/
	ENGINE_API bool resumeAll();

	/**
		@brief stops all sounds playing, 

		@param[in] timeout how long to wait in milliseconds until halting all chanells.
				   Default is instant.

		@return success -- always true
	*/
	ENGINE_API bool stopAll(int timeout = 0);

	/**
		@brief pauses a sound

		@param[in] ID of sound to pause

		@return success

		@exception could not find sound, returns false
		@exception could not play sound, returns false
	*/
	ENGINE_API bool pause(const std::string& ID);

	/**
		@brief Plays a loaded sound

		@param[in] ID of sound to play
		@param[in] loop number of times to loop. Default is 0 = play once,
				   -1 to loop unitl stopped
		@param[in] timeout time to stop playing sound in milliseconds.
				   Default is -1 = play until finished

		@return success

		@exception couldn't find sound -- returns false
		@exception failed to play sound -- returns false
	*/
	ENGINE_API bool play(const std::string& ID, int loop = 0, int timeout = -1);

	/**
		@brief Resumes a paused sound

		@param[in] ID of sound to resume

		@return success

		@exception could not find sound, returns false
	*/
	ENGINE_API bool resume(const std::string& ID);

	/**
		@brief Stops a playing/paused sound

		@param[in] ID of sound to stop
		@param[in] timeout how long to wait in milliseconds until halting the chanell.
				   Default is instant.

		@return success

		@exception could not find sound
	*/
	ENGINE_API bool stop(const std::string& ID, int timeout = 0);

private:
	logMgr logger;

	std::map<std::string,std::unique_ptr<sound>> sounds;
	
	bool good;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
