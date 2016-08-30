#pragma once

#include "..\common.h"
#include <map>

typedef void* mutex;
typedef void* cond_var;

enum mutex_status {
	mutex_locked,
	mutex_unlocked,
	mutex_timed_out,
	mutex_error
};

class Thread
{
public:
	ENGINE_API Thread();
	ENGINE_API ~Thread();

	bool init();
	bool kill();

	ENGINE_API bool add(const std::string& tID, int(*func)(void*), void* param);
	ENGINE_API bool wait(const std::string& tID, int& ret);
	ENGINE_API bool detach(const std::string& tID);

	ENGINE_API bool waitAll();
	ENGINE_API bool detachAll();

	ENGINE_API void delay(u32 ms);

	// these are just straight SDL wrappers
	ENGINE_API mutex makeMutex();
	ENGINE_API void freeMutex(mutex m);

	ENGINE_API mutex_status tryLockMutex(mutex m);
	ENGINE_API mutex_status lockMutex(mutex m);
	ENGINE_API mutex_status unlockMutex(mutex m);

	ENGINE_API cond_var makeCondVar();
	ENGINE_API void freeCondVar(cond_var v);

	ENGINE_API bool condBroadcast(cond_var v);
	ENGINE_API bool condSignal(cond_var v);
	ENGINE_API bool condWait(cond_var v, mutex m);
	ENGINE_API bool condWaitTimeout(cond_var v, mutex m, u32 ms);

private:
	std::map<std::string, void*> threads;
};

