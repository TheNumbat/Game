#pragma once

#include "..\common.h"
#include "..\thread\thread.h"
#include <vector>
#include <ostream>
#include <queue>

class Log {
	struct message {
		message(const std::string& c, const std::string& l, const std::string& m, s32 s);
		std::string context;
		std::string lvl;
		std::string msg;
		s16 sec;
	};

public:
	Log(void* en);
	~Log();

	bool init();
	bool kill();

	ENGINE_API void setContext(const std::string& c);
	ENGINE_API void enterSec();
	ENGINE_API void exitSec();
	ENGINE_API void log_Err(const std::string& c, const std::string& msg, const std::string& file, const std::string& line, s16 lvl = -1);
	ENGINE_API void log_Warn(const std::string& c, const std::string& msg, const std::string& file, const std::string& line, s16 lvl = -1);
	ENGINE_API void log_Info(const std::string& c, const std::string& msg, s16 lvl = -1);

private:
	void logRaw(message msg);

	std::vector<std::ostream*> out;
	std::string context;
	s16 sec;
	void* eng;
	bool running;
	std::queue<Log::message> mq;
	mutex qlock;
	cond_var cond;

	friend s32 logThread(void*);
};

ENGINE_API extern Log* globalLog;