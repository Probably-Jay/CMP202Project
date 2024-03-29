#include "Barrier.h"

Barrier::Barrier(const int _numberOfThreads)
	: limit(_numberOfThreads)
	, count(0)
	, barrierUseCount(0)
	, enabled(true)
{
}

void Barrier::ArriveAndWait() // suspends threads untill all have reached this point
{
	std::unique_lock<std::mutex> lock(mtx); // RAII mutex lock
	int useNumber = barrierUseCount; //  keep track of the current use of barrier (used to prevent spurious wakeup while being reusable)
	if (count >= limit || !enabled) { // if the count of waiting threads reaches the barrier limit, open
		barrierUseCount++; // next use of barrier
		count = 0; // reset count
		cv.notify_all();
	}
	else { // else increment count and set thread waiting
		count++;
		cv.wait(lock, [this, useNumber] {return useNumber < barrierUseCount; }); // prevent spurious wakeup, stop waiting if this barrier use has opened
	}
}

void Barrier::UnblockAll()
{
	barrierUseCount++; // next use of barrier
	count = 0; // reset count
	cv.notify_all();
}

void Barrier::UnblockAllAndDisable()
{
	std::unique_lock<std::mutex> lock(mtx); // mutex to let any thread mid-arrival finish that execution
	enabled = false;
	barrierUseCount++;
	cv.notify_all();
}

void Barrier::Reset()
{
	std::unique_lock<std::mutex> lock(mtx); // mutex to let any thread mid-arrival finish that execution
	count = 0;
	barrierUseCount = 0;
	enabled = true;
}
