#pragma once
#include "Semaphore.h"
#include <string>
#include <vector>
using std::string;
using std::vector;
using std::lock_guard;

template<class T>
class Channel
{
public:
	Channel<T>(int _max = -1); // sentinal value will set maxSize to max_size() of buffer
	void Write(T data);
	T Read();


private:
	const int maxSize;
	Semaphore sem;
	Semaphore emptySem; // semaphore keeping track of the empty places in the chanel

	vector<T> buffer;

	mutex mtx;
};


