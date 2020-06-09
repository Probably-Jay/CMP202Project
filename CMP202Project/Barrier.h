#pragma once
#include <mutex>
#include <condition_variable>

class Barrier
{
public:
	explicit Barrier(int _numberOfThreads);
	void ArriveAndWait();

private:

	std::mutex mtx;
	std::condition_variable cv;

	int count; // amount of threads currently waiting
	int limit; // number of threads total
	int barrierUseCount; // allow the barrier to be reused by counting the use we are currently on

};

