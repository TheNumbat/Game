#include "log.h"

#include <iostream>
#include <fstream>
#include <time.h>
#include <iomanip>

#undef logErr
#undef logWarn

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
	logMsg("Initialized log.");
}

Log::~Log() {
	logSetContext("LOG");
	logMsg("Shut down log.");
}

void Log::logMsg(const std::string& msg) {
	std::time_t t(NULL);
	time(&t);
	struct tm buf;
	localtime_s(&buf, &t);
	for (std::ostream* o : out) {
		*o << std::put_time(&buf, "%H:%M:%S");
		for (int i = 0; i < sec; i++)
			*o << "   ";
		*o <<" [ENGINE/" << context << "]: " << msg << std::endl;
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
	logger->logMsg("ERROR: " + file.substr(file.find_last_of("\\/") + 1, file.find_last_of("\\/") - file.length()) + ", line " + line + ": " + msg);
}

ENGINE_API void logWarn(const std::string& msg, const std::string& file, const std::string& line) {
	logger->logMsg("WARNING: " + file.substr(file.find_last_of("\\/") + 1, file.find_last_of("\\/") - file.length()) + ", line " + line + ": " + msg);
}

ENGINE_API void logMsg(const std::string& msg) {
	logger->logMsg(msg);
}
