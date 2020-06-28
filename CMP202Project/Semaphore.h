#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>
#include <limits>

using std::unique_lock;

using std::mutex;
using std::condition_variable;

namespace Sem {
	constexpr auto reallyBigNumber  = std::numeric_limits<int>::max() * 0.7f; // a really big number for decomisioning the semaphore
}

class Semaphore
{
public:
	Semaphore(int _initial = 0);
	~Semaphore();
	void Signal(); // increment pool count, wake up a sleeping thread on wait
	void Wait(); // suspend thread until pool count is greater than 0, decrement pool count
	void Disable(); // called at destruction to unblock any possible threads still waiting on signal
	void Reset(); // reset so can be reused
private:
	mutex poolMutex;
	condition_variable cv;
	int poolCount;
	const int initialPoolSize;
};

