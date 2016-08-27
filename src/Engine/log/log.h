#pragma once

#include "..\common.h"

#include <vector>
#include <ostream>

ENGINE_API void logInit();
ENGINE_API void logQuit();

ENGINE_API void logSetContext(const std::string& context);
ENGINE_API void logEnterSec();
ENGINE_API void logExitSec();

ENGINE_API void logErr(const std::string& msg, const std::string& file, const std::string& line);
ENGINE_API void logWarn(const std::string& msg, const std::string& file, const std::string& line);
ENGINE_API void logInfo(const std::string& msg);

class Log {
	Log();
	~Log();
	std::vector<std::ostream*> out;
	std::string context;
	int sec;

	void logInfo(const std::string& lvl, const std::string& msg);

	friend void logInit();
	friend void logQuit();
	friend void logSetContext(const std::string& context);
	friend void logEnterSec();
	friend void logExitSec();
	friend void logErr(const std::string& msg, const std::string& file, const std::string& line);
	friend void logWarn(const std::string& msg, const std::string& file, const std::string& line);
	friend void logInfo(const std::string& msg);
};
extern Log* logger;

#define logErr(a) logErr(a,(std::string)__FILE__,std::to_string(__LINE__))
#define logWarn(a) logWarn(a,(std::string)__FILE__,std::to_string(__LINE__))

