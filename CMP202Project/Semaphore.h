#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>

using std::unique_lock;

using std::mutex;
using std::condition_variable;

class Semaphore
{
public:
	Semaphore(int _initial = 0);
	void Signal(); // increment pool count, wake up a sleeping thread on wait
	void Wait(); // suspend thread until pool count is greater than 0, decrement pool count
private:
	mutex poolMutex;
	condition_variable cv;
	int poolCount;
};

