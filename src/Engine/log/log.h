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

// You must call this ONCE, AT THE TOP, of each function you want to use logging in
#define logSetContext(a) const std::string LOG_CONTEXT(a)

#define logErr(a) globalLog->logErr(LOG_CONTEXT,a,(std::string)__FILE__,std::to_string(__LINE__))
#define logWarn(a) globalLog->logWarn(LOG_CONTEXT,a,(std::string)__FILE__,std::to_string(__LINE__))
#define logInfo(a) globalLog->logInfo(LOG_CONTEXT,a)
#define logEnterSec() globalLog->enterSec()
#define logExitSec() globalLog->exitSec()

ENGINE_API extern Log* globalLog;
