#include "log.h"

#include <iostream>
#include <fstream>
#include <time.h>
#include <iomanip>
#include "..\game.h"

Log* globalLog;

#ifdef FLOG
std::ofstream fout(FLOG);
#endif // FLOG

s32 logThread(void* data) {
	game* g = (game*)data;
	while (g->log.running) {
		g->thread.lockMutex(g->log.qlock);
		while (g->log.mq.size()) {
			Log::message m = g->log.mq.front();
			g->log.mq.pop();
			g->log.logRaw(m);
		}
		g->thread.condWait(g->log.cond, g->log.qlock);
		g->thread.unlockMutex(g->log.qlock);
	}
	return 0;
}

Log::message::message(const std::string& c, const std::string& l, const std::string& m, s32 s) {
	context = c;
	lvl = l;
	msg = m;
	sec = s;
}

Log::Log(void* game) {
	g = game;
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

	game* _g = (game*) g;
	qlock = _g->thread.makeMutex();
	cond = _g->thread.makeCondVar();
	running = true;
	_g->thread.add("log", &logThread, g);

	logRaw(message("LOG","INFO", "Initialized log.",sec));
	return true;
}

bool Log::kill() {
	game* _g = (game*) g;
	int buf;

	_g->thread.lockMutex(qlock);
	running = false;
	_g->thread.condSignal(cond);
	_g->thread.unlockMutex(qlock);
	_g->thread.wait("log",buf);
	_g->thread.freeMutex(qlock);
	_g->thread.freeCondVar(cond);
	logRaw(message("LOG","INFO", "Shut down log.",sec));
	return true;
}

void Log::setContext(const std::string& c) {
	game* _g = (game*) g;
	_g->thread.lockMutex(qlock);
	context = c;
	_g->thread.unlockMutex(qlock);
}

void Log::enterSec() {
	game* _g = (game*) g;
	_g->thread.lockMutex(qlock);
	sec++;
	_g->thread.unlockMutex(qlock);
}

void Log::exitSec() {
	game* _g = (game*) g;
	_g->thread.lockMutex(qlock);
	sec--;
	_g->thread.unlockMutex(qlock);
}

void Log::log_Err(const std::string& c, const std::string& msg, const std::string& file, const std::string& line, s16 lvl) {
	game* _g = (game*) g;
	_g->thread.lockMutex(qlock);
	mq.push(message(c,"ERROR", file.substr(file.find_last_of("\\/") + 1, file.find_last_of("\\/") - file.length()) + ", line " + line + ": " + msg, lvl == -1 ? sec : lvl));
	_g->thread.condSignal(cond);
	_g->thread.unlockMutex(qlock);
}

void Log::log_Warn(const std::string& c, const std::string& msg, const std::string& file, const std::string& line, s16 lvl) {
	game* _g = (game*) g;
	_g->thread.lockMutex(qlock);
	mq.push(message(c,"WARNING", file.substr(file.find_last_of("\\/") + 1, file.find_last_of("\\/") - file.length()) + ", line " + line + ": " + msg, lvl == -1 ? sec : lvl));
	_g->thread.condSignal(cond);
	_g->thread.unlockMutex(qlock);
}

void Log::log_Info(const std::string& c, const std::string& msg, s16 lvl) {
	game* _g = (game*) g;
	_g->thread.lockMutex(qlock);
	mq.push(message(c,"INFO", msg, lvl == -1 ? sec : lvl));
	_g->thread.condSignal(cond);
	_g->thread.unlockMutex(qlock);
}
