#include "Channel.h"
#pragma once


template<class T>
Channel<T>::Channel(int _max)
	: maxSize((_max > 0 && _max <= INT_MAX - 1) ? _max : INT_MAX- 1) // bounds just to be safe
	, sem(0)
	, emptySem(maxSize)
	, enabled(true)

{
}


template<class T>
Channel<T>::~Channel()
{
	UnblockAllandDisable();
	buffer.clear();
}


template<class T>
void Channel<T>::Write(T data)
{
	emptySem.Wait(); // block and suspend unless there is room to write into the buffer
	{ // RAII scope
		lock_guard<mutex> lock(mtx);
		buffer.push_front(data); 
	}
	sem.Signal(); 

}

template<class T>
T Channel<T>::Read()
{
	sem.Wait(); // block and suspend unless there are available elements in the buffer
	lock_guard<mutex> lock(mtx); // RAII mutex for readind and altering the buffer
	if (!enabled) return T{}; // if channel has been decommissioned, unblock but prevent reading empty buffer
	T item = buffer.back();
	buffer.pop_back(); // remove read item from buffer 
	emptySem.Signal(); // signal that there is now extra room in the buffer
	return item;
}

template<class T>
void Channel<T>::UnblockAllandDisable()
{
	if (enabled) {
		enabled = false;
		Clear();
		sem.Disable();
		emptySem.Disable();
	}
}

template<class T>
void Channel<T>::Clear()
{
	lock_guard<mutex> lock(mtx);
	buffer.clear();
	sem.Reset();
	emptySem.Reset();
}

template<class T>
void Channel<T>::Reset()
{
	Clear();
	enabled = true;
}
