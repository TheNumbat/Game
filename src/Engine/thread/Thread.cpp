
#include "thread.h"
#include "..\log\log.h"
#include <SDL.h>
#include <SDL_thread.h>

// TODO: add logging back to mutex/cond_var functions in a way that doesn't break

Thread::Thread() {

}

Thread::~Thread() {

}

bool Thread::init() {
	logSetContext("THREAD");
	logInfo("Threading initialized.");
	return true;
}

bool Thread::kill() {
	logSetContext("THREAD");
	logInfo("Threading destroyed.");
	return true;
}

bool Thread::add(const std::string& tID, int(*func)(void*), void* param) {
	logSetContext("THREAD");

	SDL_Thread* newT = SDL_CreateThread(func, tID.c_str(), param);
	assert(newT);
	if (!newT) {
		logWarn("Could not create thread ID " + tID + ", Error: " + SDL_GetError());
		return false;
	}

	logInfo("Created thread ID " + tID);
	threads.insert({tID, newT});
	return true;
}

bool Thread::wait(const std::string& tID, int& ret) {
	logSetContext("THREAD");

	logInfo("Wating on thread ID " + tID);
	auto entry = threads.find(tID);
	if (entry == threads.end()) {
		logWarn("Could not find thread to wait on, ID " + tID);
		return false;
	}

	SDL_WaitThread((SDL_Thread*)entry->second, &ret);
	threads.erase(entry);
	return true;
}

bool Thread::detach(const std::string& tID) {
	logSetContext("THREAD");

	auto entry = threads.find(tID);
	if(entry == threads.end()) {
		logWarn("Could not find thread to detach, ID " + tID);
		return false;
	}

	logInfo("Detached thread ID " + tID);
	SDL_DetachThread((SDL_Thread*)entry->second);
	threads.erase(entry);
	return true;
}

bool Thread::waitAll() {
	logSetContext("THREAD");

	logInfo("Waiting on all threads");

	int temp;
	while (threads.size()) {
		wait(threads.begin()->first,temp);
	}

	return true;
}

bool Thread::detachAll() {
	logSetContext("THREAD");

	logInfo("Detaching on all threads");

	while (threads.size()) {
		detach(threads.begin()->first);
	}

	return true;
}

void Thread::delay(u32 ms) {
	SDL_Delay(ms);
}

mutex Thread::makeMutex() {
	return SDL_CreateMutex();
}

void Thread::freeMutex(mutex m) {
	SDL_DestroyMutex((SDL_mutex*)m);
}

mutex_status Thread::tryLockMutex(mutex m) {

	int result = SDL_TryLockMutex((SDL_mutex*)m);
	if(result == 0) return mutex_locked;
	if(result == SDL_MUTEX_TIMEDOUT) return mutex_timed_out;

	return mutex_error;
}

mutex_status Thread::lockMutex(mutex m) {

	int result = SDL_LockMutex((SDL_mutex*)m);
	if(result == 0) return mutex_locked;

	return mutex_error;
} 

mutex_status Thread::unlockMutex(mutex m) {

	int result = SDL_UnlockMutex((SDL_mutex*)m);
	if(result == 0) return mutex_unlocked;

	return mutex_error;
} 

cond_var Thread::makeCondVar() {
	return SDL_CreateCond();
}

void Thread::freeCondVar(cond_var v) {
	SDL_DestroyCond((SDL_cond*)v);
}

bool Thread::condBroadcast(cond_var v) {

	int result = SDL_CondBroadcast((SDL_cond*)v);
	assert(result == 0);
	if (result != 0) {
		return false;
	}

	return true;
}

bool Thread::condSignal(cond_var v) {

	int result = SDL_CondSignal((SDL_cond*)v);
	assert(result == 0);
	if (result != 0) {
		return false;
	}

	return true;
}

bool Thread::condWait(cond_var v, mutex m) {

	int result = SDL_CondWait((SDL_cond*)v, (SDL_mutex*)m);
	assert(result == 0);
	if (result != 0) {
		return false;
	}

	return true;
}

bool Thread::condWaitTimeout(cond_var v, mutex m, u32 ms) {

	int result = SDL_CondWaitTimeout((SDL_cond*)v, (SDL_mutex*)m, ms ? ms : SDL_MUTEX_MAXWAIT);
	assert(result == 0);
	if (result != 0) {
		return false;
	}

	return true;
}
