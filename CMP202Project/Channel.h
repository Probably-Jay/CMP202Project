#pragma once
#include "Semaphore.h"
#include <string>
#include <deque>
using std::string;
using std::deque;
using std::lock_guard;

template<class T>
class Channel
{
public:
	Channel<T>(int _max = -1); // sentinal value will set maxSize to max_size() of buffer
	~Channel();

	//Channel(const Channel &);
	//Channel& operator=(const Channel& c);

	void Write(T data); // add a value to the channel (if there is room) signal that a value has been added to wake up sleeping threads
	T Read(); // try read a value from the channel, suspend execution until not empty

	void UnblockAllandDisable(); // called at destruction to unblock any possible threads still attempting to read / write

	void Clear();

	void Reset();
private:

	bool enabled;
	
	const int maxSize;
	Semaphore sem;
	Semaphore emptySem; // semaphore keeping track of the empty places in the chanel

	deque<T> buffer;

	mutex mtx;
};


#include "Channel.tpp"