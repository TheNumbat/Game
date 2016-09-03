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
	Thread();
	~Thread();

	bool init();
	bool kill();

	bool add(const std::string& tID, int(*func)(void*), void* param);
	bool wait(const std::string& tID, int& ret);
	bool detach(const std::string& tID);

	bool waitAll();
	bool detachAll();

	void delay(u32 ms);

	// these are just straight SDL wrappers
	mutex makeMutex();
	void freeMutex(mutex m);

	mutex_status tryLockMutex(mutex m);
	mutex_status lockMutex(mutex m);
	mutex_status unlockMutex(mutex m);

	cond_var makeCondVar();
	void freeCondVar(cond_var v);

	bool condBroadcast(cond_var v);
	bool condSignal(cond_var v);
	bool condWait(cond_var v, mutex m);
	bool condWaitTimeout(cond_var v, mutex m, u32 ms);

private:
	std::map<std::string, void*> threads;
};

