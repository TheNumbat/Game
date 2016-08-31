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
	#define logWarn(a) 
	#define logInfo(a)
#elif VERBOSE == 1
	#define logErr(a) globalLog->logErr(LOG_CONTEXT,a,(std::string)__FILE__,std::to_string(__LINE__))
	#define logWarn(a) 
	#define logInfo(a)
#elif VERBOSE == 2
	#define logErr(a) globalLog->logErr(LOG_CONTEXT,a,(std::string)__FILE__,std::to_string(__LINE__))
	#define logWarn(a) globalLog->logWarn(LOG_CONTEXT,a,(std::string)__FILE__,std::to_string(__LINE__))
	#define logInfo(a)
#elif VERBOSE == 3
	#define logErr(a) globalLog->logErr(LOG_CONTEXT,a,(std::string)__FILE__,std::to_string(__LINE__))
	#define logWarn(a) globalLog->logWarn(LOG_CONTEXT,a,(std::string)__FILE__,std::to_string(__LINE__))
	#define logInfo(a) globalLog->logInfo(LOG_CONTEXT,a)
#endif// VERBOSE

#ifdef LOGCONTEXT
	// Call this ONCE at the start of a function
	#define logSetContext(a) const std::string LOG_CONTEXT(a)
#else
	#define setLogContext(a)
#endif // CONTEXT
