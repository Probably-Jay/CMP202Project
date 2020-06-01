#include "Channel.h"


template<class T>
Channel<T>::Channel(int _max)
	:	maxSize((_max > 0 && _max <= INT_MAX - 1) ? _max : INT_MAX- 1) // bounds just to be safe
	,	sem(0)
	,	emptySem(maxSize)

{
}


template<class T>
void Channel<T>::Write(T data)
{
	emptySem.Wait(); // block and suspend unless there is room to write into the buffer
	lock_guard<mutex> lock(mtx);
	buffer.push_back(data);
	sem.Signal(); 

}


template<class T>
T Channel<T>::Read()
{
	sem.Wait(); // block and suspend unless there are available elements in the buffer
	lock_guard<mutex> lock(mtx);
	T item = buffer.back();
	buffer.pop_back();
	emptySem.Signal();
	return item;
}