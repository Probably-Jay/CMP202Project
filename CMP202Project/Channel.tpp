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

//template<class T>
//Channel<T>::Channel(const Channel & c)
//	: maxSize(c.maxSize)
//	, sem(c.sem)
//	, emptySem(c.emptySem)
//	, enabled(c.enabled)
//{
//}


template<class T>
Channel<T>::~Channel()
{
	UnblockAllandDisable();
	buffer.clear();
}

//template<class T>
//Channel<T>& Channel<T>::operator=(const Channel& c)
//{
//	maxSize = c.maxSize;
//	sem = c.sem;
//	emptySem = c.emptySem;
//	enabled = c.enabled;
//	return *this;
//}

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
	if (!enabled) return T(); // if channel has been decommissioned, prevent reading empty buffer
	lock_guard<mutex> lock(mtx);
	T item = buffer.back();
	buffer.pop_back();
	emptySem.Signal();
	return item;
}

template<class T>
void Channel<T>::UnblockAllandDisable()
{
	if (enabled) {
		enabled = false;
		sem.Disable();
		emptySem.Disable();
	}
}

template<class T>
void Channel<T>::Clear()
{
	buffer.clear();
	sem.Reset();
	emptySem.Reset(maxSize);
}
