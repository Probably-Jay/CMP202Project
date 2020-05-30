#include "Semaphore.h"

Semaphore::Semaphore(int _initial)
	:poolCount(_initial)
{

}

void Semaphore::Signal()
{
	{ // mutex scope
		unique_lock<mutex> lock(mut); // aquire mutex lock as required by condition variable, RAII
		
		poolCount++;
		
	}
	cv.notify_one(); // we have only added one to the pool count so we only need to notify one thread to wake up and deal with it
}

void Semaphore::Wait()
{
	unique_lock<mutex> lock(mut); // aquire mutex lock as required by condition variable, RAII
	cv.wait(lock, [this] {return poolCount > 0;});	// suspend thread if pool is empty until notified and pool is no longer empty
													// this automatically releases the lock while thread is suspended
	poolCount--;
	return;
}
