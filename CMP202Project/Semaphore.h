#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

using std::unique_lock;

using std::mutex;
using std::condition_variable;

class Semaphore
{
public:
	Semaphore(int _initial = 0, int _max = INT_MAX);
	void Signal();
	void Wait();
private:
	mutex mut;
	condition_variable cv;
	int poolCount;
	int maxPoolSize;
};

