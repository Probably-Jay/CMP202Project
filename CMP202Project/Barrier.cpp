#include "Barrier.h"

Barrier::Barrier(const int _numberOfThreads)
	: limit(_numberOfThreads)
	, count(0)
	, barrierUseCount(0)
{
}

void Barrier::ArriveAndWait() // suspends threads untill all have reached this point
{
	std::unique_lock<std::mutex> lock(mtx); // RAII mutex lock
	int useNumber = barrierUseCount; //  keep track of the current use of barrier (used to prevent spurious wakeup while being reusable)
	if (count >= limit) { // if the count of waiting threads reaches the barrier limit, open
		UnblockAll();
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
