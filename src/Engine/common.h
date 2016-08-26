#pragma once

// Debug Defines
#define ASSERTS

// Logging Defines
#define VERBOSE 3			// 0 = nothing, 1 = errors, 2 = warnings, 3 = everything
#define CLOG				// log to console
#define FLOG "log.txt"		// log to file
#define LOGCONTEXT				// log contexts
#define TIMESTAMPS			// log time stamps

#include <string>
#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////

#ifdef ENGINE_EXPORT
	#define ENGINE_API __declspec(dllexport)
#else
	#define ENGINE_API __declspec(dllimport)
#endif // ENGINE_EXPORT

#ifdef ASSERTS
	#include <assert.h>
#else
	#define assert(a) 
#endif // ASSERTS

#if VERBOSE == 0
	#define logErr(a)
	#define logWarn(a) 
	#define logMsg(a)
#elif VERBOSE == 1
	#define logWarn(a) 
	#define logMsg(a)
#elif VERBOSE == 2
	#define logMsg(a)
#endif // VERBOSE

#ifndef CONTEXT
	#define setLogContext(a)
#endif // CONTEXT

typedef float r32;
typedef double r64;
typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t  i64;
typedef uint64_t u64;
