#include "log.h"

#include <iostream>
#include <fstream>
#include <time.h>
#include <iomanip>

#undef logErr
#undef logWarn

// TODO Push log output onto queue for another thread

Log* logger;

#ifdef FLOG
	std::ofstream fout(FLOG);
#endif // FLOG

Log::Log() {
	context = "LOG";
	sec = 0;
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
	logMsg("INFO", "Initialized log.");
}

Log::~Log() {
	logSetContext("LOG");
	logMsg("INFO", "Shut down log.");
}

void Log::logMsg(const std::string& lvl, const std::string& msg) {
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
		for (int i = 0; i < sec; i++)
			*o << "   ";
		*o << " [" << lvl;
#ifdef LOGCONTEXT
		*o << "\\" << context;
#endif // LOGCONTEXT
		*o << "]: " << msg << std::endl;
	}
}

ENGINE_API void logInit() {
	logger = new Log();
}

ENGINE_API void logQuit() {
	delete logger;
}

ENGINE_API void logSetContext(const std::string& context) {
	logger->context = context;
}

ENGINE_API void logEnterSec() {
	logger->sec++;
}

ENGINE_API void logExitSec() {
	logger->sec--;
}

ENGINE_API void logErr(const std::string& msg, const std::string& file, const std::string& line) {
	logger->logMsg("ERROR", file.substr(file.find_last_of("\\/") + 1, file.find_last_of("\\/") - file.length()) + ", line " + line + ": " + msg);
}

ENGINE_API void logWarn(const std::string& msg, const std::string& file, const std::string& line) {
	logger->logMsg("WARNING", file.substr(file.find_last_of("\\/") + 1, file.find_last_of("\\/") - file.length()) + ", line " + line + ": " + msg);
}

ENGINE_API void logMsg(const std::string& msg) {
	logger->logMsg("INFO", msg);
}
