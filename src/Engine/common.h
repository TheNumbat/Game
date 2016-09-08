#pragma once

// Debug Defines
#define ENGINE_ASSERTS

// Logging Defines
#define VERBOSE		3				// 0 = nothing, 1 = errors, 2 = warnings, 3 = info
#define CLOG						// log to console
#define FLOG		"log.txt"		// log to file
#define LOGCONTEXT					// log contexts
#define TIMESTAMPS					// log time stamps

#include <string>
#include <stdint.h>

typedef float		r32;
typedef double		r64;
typedef int8_t		s8;
typedef uint8_t		u8;
typedef int16_t		s16;
typedef uint16_t	u16;
typedef int32_t		s32;
typedef uint32_t	u32;
typedef int64_t		s64;
typedef uint64_t	u64;

///////////////////////////////////////////////////////////////////////////////

#ifdef ENGINE_EXPORT
	#define ENGINE_API __declspec(dllexport)
#else
	#define ENGINE_API __declspec(dllimport)
#endif // ENGINE_EXPORT

#ifdef ENGINE_ASSERTS
	#include <assert.h>
#else
	#define assert(a) 
#endif // ASSERTS

#define logEnterSec() globalLog->enterSec()
#define logExitSec() globalLog->exitSec()

#if VERBOSE == 0
	#define logErr(a)
	#define logErrLvl(a,b)
	#define logWarn(a) 
	#define logWarnLvl(a,b)
	#define logInfo(a)
	#define logInfoLvl(a,b)
#elif VERBOSE == 1
	#define logErr(a) globalLog->log_Err(LOG_CONTEXT,a,(std::string)__FILE__,std::to_string(__LINE__))
	#define logErrLvl(a,b) globalLog->log_Err(LOG_CONTEXT,a,(std::string)__FILE__,std::to_string(__LINE__),b)
	#define logWarn(a) 
	#define logWarnLvl(a,b)
	#define logInfo(a)
	#define logInfoLvl(a,b)
#elif VERBOSE == 2
	#define logErr(a) globalLog->log_Err(LOG_CONTEXT,a,(std::string)__FILE__,std::to_string(__LINE__))
	#define logErrLvl(a,b) globalLog->log_Err(LOG_CONTEXT,a,(std::string)__FILE__,std::to_string(__LINE__),b)
	#define logWarn(a) globalLog->log_Warn(LOG_CONTEXT,a,(std::string)__FILE__,std::to_string(__LINE__))
	#define logWarnLvl(a,b) globalLog->log_Warn(LOG_CONTEXT,a,(std::string)__FILE__,std::to_string(__LINE__),b)
	#define logInfo(a)
	#define logInfoLvl(a,b)
#elif VERBOSE == 3
	#define logErr(a) globalLog->log_Err(LOG_CONTEXT,a,(std::string)__FILE__,std::to_string(__LINE__))
	#define logErrLvl(a,b) globalLog->log_Err(LOG_CONTEXT,a,(std::string)__FILE__,std::to_string(__LINE__),b)
	#define logWarn(a) globalLog->log_Warn(LOG_CONTEXT,a,(std::string)__FILE__,std::to_string(__LINE__)) 
	#define logWarnLvl(a,b) globalLog->log_Warn(LOG_CONTEXT,a,(std::string)__FILE__,std::to_string(__LINE__),b)
	#define logInfo(a) globalLog->log_Info(LOG_CONTEXT,a)
	#define logInfoLvl(a,b) globalLog->log_Info(LOG_CONTEXT,a,b)
#endif // VERBOSE

#ifdef LOGCONTEXT
	// Call this ONCE at the start of each function that uses logging
	#define logSetContext(a) std::string LOG_CONTEXT(a)
	#define logChangeContext(a) LOG_CONTEXT = a
#else
	#define logSetContext(a)
	#define logChangeContext(a)
	#define LOG_CONTEXT ""
#endif // CONTEXT
