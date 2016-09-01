#include "log.h"

#include <iostream>
#include <fstream>
#include <time.h>
#include <iomanip>
#include "..\engine.h"

#undef logErr
#undef logWarn
#undef logSetContext
#undef logEnterSec
#undef logExitSec
#undef logInfo

Log* globalLog;

#ifdef FLOG
std::ofstream fout(FLOG);
#endif // FLOG

s32 logThread(void* data) {
	engine* e = (engine*)data;
	while (e->log.running) {
		e->thread.lockMutex(e->log.qlock);
		if (e->log.mq.size()) {
			Log::message m = e->log.mq.front();
			e->log.mq.pop();
			e->log.logRaw(m);
		}
		e->thread.condWait(e->log.cond, e->log.qlock);
		e->thread.unlockMutex(e->log.qlock);
	}
	return 0;
}

Log::message::message(const std::string& c, const std::string& l, const std::string& m, s32 s) {
	context = c;
	lvl = l;
	msg = m;
	sec = s;
}

Log::Log(void* en) {
	eng = en;
	context = "LOG";
	sec = 0;
	globalLog = this;
}

Log::~Log() {

}

void Log::logRaw(Log::message msg) {
#ifdef TIMESTAMPS
	std::time_t t(NULL);
	time(&t);
	struct tm buf;
	localtime_s(&buf, &t);
#endif // TIMESTAMPS
	for (std::ostream* o : out) {
#ifdef TIMESTAMPS
		*o << std::put_time(&buf, "%H:%M:%S");
#endif // TIMESTAMPS
		for (u8 i = 0; i < msg.sec; i++)
			*o << "   ";
		*o << " [" << msg.lvl;
#ifdef LOGCONTEXT
		*o << "\\" << msg.context;
#endif // LOGCONTEXT
		*o << "]: " << msg.msg << std::endl;
	}
}

bool Log::init() {
#ifdef CLOG
	out.push_back(&std::cout);
#endif // CLOG
#ifdef FLOG
	out.push_back(&fout);
#endif // FLOG
	for (std::ostream* o : out) {
		*o << std::endl << std::endl
			<< "   ENGINE LOG STARTUP" << std::endl
			<< "   ==================" << std::endl
			<< std::endl;
	}

	engine* e = (engine*)eng;
	qlock = e->thread.makeMutex();
	cond = e->thread.makeCondVar();
	running = true;
	e->thread.add("log", &logThread, eng);

	logRaw(message("LOG","INFO", "Initialized log.",sec));
	return true;
}

bool Log::kill() {
	engine* e = (engine*)eng;
	int buf;

	e->thread.lockMutex(qlock);
	running = false;
	e->thread.condSignal(cond);
	e->thread.unlockMutex(qlock);
	e->thread.wait("log",buf);
	e->thread.freeMutex(qlock);
	e->thread.freeCondVar(cond);
	logRaw(message("LOG","INFO", "Shut down log.",sec));
	return true;
}

void Log::setContext(const std::string& c) {
	engine* e = (engine*)eng;
	e->thread.lockMutex(qlock);
	context = c;
	e->thread.unlockMutex(qlock);
}

void Log::enterSec() {
	engine* e = (engine*)eng;
	e->thread.lockMutex(qlock);
	sec++;
	e->thread.unlockMutex(qlock);
}

void Log::exitSec() {
	engine* e = (engine*)eng;
	e->thread.lockMutex(qlock);
	sec--;
	e->thread.unlockMutex(qlock);
}

void Log::log_Err(const std::string& c, const std::string& msg, const std::string& file, const std::string& line, s16 lvl) {
	engine* e = (engine*)eng;
	e->thread.lockMutex(qlock);
	mq.push(message(c,"ERROR", file.substr(file.find_last_of("\\/") + 1, file.find_last_of("\\/") - file.length()) + ", line " + line + ": " + msg, lvl == -1 ? sec : lvl));
	e->thread.condSignal(cond);
	e->thread.unlockMutex(qlock);
}

void Log::log_Warn(const std::string& c, const std::string& msg, const std::string& file, const std::string& line, s16 lvl) {
	engine* e = (engine*)eng;
	e->thread.lockMutex(qlock);
	mq.push(message(c,"WARNING", file.substr(file.find_last_of("\\/") + 1, file.find_last_of("\\/") - file.length()) + ", line " + line + ": " + msg, lvl == -1 ? sec : lvl));
	e->thread.condSignal(cond);
	e->thread.unlockMutex(qlock);
}

void Log::log_Info(const std::string& c, const std::string& msg, s16 lvl) {
	engine* e = (engine*)eng;
	e->thread.lockMutex(qlock);
	mq.push(message(c,"INFO", msg, lvl == -1 ? sec : lvl));
	e->thread.condSignal(cond);
	e->thread.unlockMutex(qlock);
}
