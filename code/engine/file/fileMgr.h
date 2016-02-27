// Program Information ////////////////////////////////////////////////////////

/**
	@file fileMgr.h

	@brief Declartion file for file manager class.

	Event manager stores and translates events.

	@author Max Slater

	@version 1.00 (28 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include "log/logMgr.h"
#include "file/file.h"

#include <map>
#include <memory>

// Global constant definitions  ///////////////////////////////////////////////

/**
	@brief Describes where to offset from within a file

	Used by fileMgr.
*/
enum fileseek
{
	file_start,
	file_current,
	file_end
};

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Manges file IO

	Used for all file operations.
*/
class fileMgr
{
public:

	/**
		@brief fileMgr constructor
		
		No need to initialize, as it will work if SDL is initialized
	*/
	ENGINE_API fileMgr();

	/**
		@brief fileMgr constructor
		
		No need to deinitialize other than delete file handle things
	*/
	ENGINE_API ~fileMgr();

	/**
		@brief loads a file into the file map

		Loads file.

		@param[in] ID of file to use or empty to use file name
		@param[in] path location of file
		@param[in] type whether to open the file as text or binary
		@param[in] access what access level to open the file with

		@return success

		@expcetion File ID already in use, returns false
		@exception Unable to open file (variety of reasons), will return false
	*/
	ENGINE_API bool loadFile(const std::string& path, int8 type, const std::string& access, const std::string& ID = "");

	/**
		@brief frees file

		Deallocates file.

		@param[in] ID of file to free

		@return success

		@exception file is not loaded, returns false
	*/
	ENGINE_API bool freeFile(const std::string& ID);

	/**
		@brief Reads data from a file

		@param[in] ID of file to use
		@param[in] buffer location to read data into
		@param[in] size (in bytes) of each object to read, or empty for sizeof(buffer)
		@param[in] numobjs the maximum number of objects of size size to read, default is one

		@return success

		@expcetion unable to read from file, returns false
	*/
	ENGINE_API bool read(const std::string& ID, void* buffer, uint32 size, uint32 numobjs = 1);

	/**
		@brief Writes data to a file

		@param[in] ID of file to use
		@param[in] buffer location to write data from
		@param[in] size (in bytes) of each object to write, or empty for sizeof(buffer)
		@param[in] numobjs the number of objects of size size to write, default is one

		@return success

		@expcetion unable to write to file, returns false
	*/
	ENGINE_API bool write(const std::string& ID, void* buffer, uint32 size, uint32 numobjs = 1);

	/**
		@brief Gets the offset (in bytes) of the read/write cursor from the start of the file

		@param[in] ID of file to use

		@return success

		@expcetion unable to retreive offset, returns -1
	*/
	ENGINE_API uint64 getOffset(const std::string& ID);

	/**
		@brief sets the offset within a file (seeks)

		@param[in] ID of file to use
		@param[in] offset to go to
		@param[in] start where to start from, or empty to seek from the start

		@return new offset or -1 for failure

		@expcetion unable to seek in file, returns -1
	*/
	ENGINE_API uint64 setOffset(const std::string& ID, uint64 offset, fileseek start = file_start);

private:
	std::map<std::string,std::unique_ptr<file>> files;

	logMgr logger;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
