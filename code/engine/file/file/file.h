// Program Information ////////////////////////////////////////////////////////

/**
	@file file.h

	@brief Declartion file for file class

	file stores information needed to display a file

	@author Max Slater

	@version 1.00 (31 Jan 2016)
        Created
*/

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "engine_common.h"

#include <string>
#include <memory>

// Global constant definitions  ///////////////////////////////////////////////

// File types
/// Open file as text
const int8 file_text = 't';
/// Open file as binary
const int8 file_binary = 'b';

// File access types
/// Open file to read only
const std::string file_read = "r";
/// Open empty file to write only
const std::string file_empty_write = "w";
/// Open file to append only
const std::string file_append = "a";
/// Open file to read and write
const std::string file_read_write = "r+";
/// Open empty file to read and write
const std::string file_empty_read_write = "w+";
/// Open file to read and append only
const std::string file_read_append = "a+";

// Class/Struct definitions  //////////////////////////////////////////////////

/**
	@brief Abtracts an SDL file

	Holds data for an sdl file

	Used by fileMgr.
*/
class file
{
	/**
		@brief file constructor

		Sets file to uninititialized
	*/
	file();

	/**
		@brief file destructor

		Frees the file, calls free()
	*/
	~file();

	/**
		@brief loads file

		Loads file.

		@param[in] path location of file
		@param[in] type whether to open the file as text or binary
		@param[in] access what access level to open the file with

		@return success

		@exception Unable to open file (variety of reasons), will return false
	*/
	bool load(const std::string& path, int8 type, const std::string& access);

	/**
		@brief Frees file

		Deallocates file

		@return success

		@exception if file is already freed does nothing, returns false
	*/
	bool free();

	/// Pointer to SDL file struct
	void* sdl_file;
	/// If file is good
	bool good;

	friend class fileMgr;

	/// Friendship for smart pointers
	friend class std::default_delete<file>;
	/// Friendship for smart pointers
	friend std::unique_ptr<file> std::make_unique<file>();
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
