#include "Semaphore.h"

Semaphore::Semaphore(int _initial)
	:	poolCount((_initial>=0&&_initial<=INT_MAX-1)?_initial:0)
	,	initialPoolSize(_initial)
{

}

Semaphore::~Semaphore()
{
	Disable(); // save any blocking thread from possibly getting stuck
}

// increment pool count, wake up a sleeping thread on wait
void Semaphore::Signal()
{
	{ // mutex scope
		unique_lock<mutex> lock(poolMutex); // aquire mutex lock as required by condition variable, RAII
		poolCount++;
		
	}
	cv.notify_one(); // we have only added one to the pool count so we only need to notify one thread to wake up and deal with it
}

// suspend thread until pool count is greater than 0, decrement pool count
void Semaphore::Wait()
{
	unique_lock<mutex> lock(poolMutex); // aquire mutex lock as required by condition variable, RAII
	cv.wait(lock, [this] {return poolCount > 0;});	// suspend thread if pool is empty until notified and pool is no longer empty
													// this automatically releases the lock while thread is suspended
	poolCount--;
	return;
}

void Semaphore::Disable() // unblock any blocked threads at the end of this objects life
{
	unique_lock<mutex> lock(poolMutex); // aquire mutex lock as required by condition variable, RAII
	poolCount = Sem::reallyBigNumber;// a really big number
	cv.notify_all();
}

void Semaphore::Reset()
{
	Disable(); // clear it out
	unique_lock<mutex> lock(poolMutex);
	poolCount = initialPoolSize;
	cv.notify_all();
}
