#pragma once

#include "..\common.h"
#include "..\thread\thread.h"
#include <vector>
#include <ostream>
#include <queue>

// TODO: probably shouldn't do this
#pragma push_macro("logErr")
#pragma push_macro("logWarn")
#pragma push_macro("logSetContext")
#pragma push_macro("logEnterSec")
#pragma push_macro("logExitSec")
#pragma push_macro("logInfo")
#undef logErr
#undef logWarn
#undef logSetContext
#undef logEnterSec
#undef logExitSec
#undef logInfo

class Log {
	struct message {
		message(const std::string& c, const std::string& l, const std::string& m, s32 s);
		std::string context;
		std::string lvl;
		std::string msg;
		s32 sec;
	};

public:
	Log(void* en);
	~Log();

	bool init();
	bool kill();

	ENGINE_API void setContext(const std::string& c);
	ENGINE_API void enterSec();
	ENGINE_API void exitSec();
	ENGINE_API void logErr(const std::string& c, const std::string& msg, const std::string& file, const std::string& line);
	ENGINE_API void logWarn(const std::string& c, const std::string& msg, const std::string& file, const std::string& line);
	ENGINE_API void logInfo(const std::string& c, const std::string& msg);

private:
	void logRaw(message msg);

	std::vector<std::ostream*> out;
	std::string context;
	s32 sec;
	void* eng;
	bool running;
	std::queue<Log::message> mq;
	mutex qlock;
	cond_var cond;

	friend s32 logThread(void*);
};

ENGINE_API extern Log* globalLog;

#pragma pop_macro("logErr")
#pragma pop_macro("logWarn")
#pragma pop_macro("logSetContext")
#pragma pop_macro("logEnterSec")
#pragma pop_macro("logExitSec")
#pragma pop_macro("logInfo")
