#include "Semaphore.h"

Semaphore::Semaphore(int _initial, int _max)
	:poolCount(_initial), maxPoolSize(_max)
{

}

void Semaphore::Signal()
{
	{ // mutex scope
		unique_lock<mutex> lock(mut); // aquire mutex lock as required by condition variable, RAII
		if(poolCount < maxPoolSize)
			poolCount++;
	}
	cv.notify_one();
}

void Semaphore::Wait()
{
	unique_lock<mutex> lock(mut); // aquire mutex lock as required by condition variable, RAII
	cv.wait(lock, [this] {return poolCount > 0;});	// suspend thread if pool is empty until notified and pool is no longer empty
													// this automatically releases the lock while thread is suspended
													/// this might need a chack if not empty duno what itl do
	poolCount--;
	return;
}
