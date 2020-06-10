#include "PasswordGeneratorThreadWrapper.h"


PasswordGeneratorThreadWrapper::PasswordGeneratorThreadWrapper(Channel<std::string>* _passwordChannel,Barrier*_barrier)
	: passwordChannel(_passwordChannel)
	, barrier(_barrier)
	, segMin(NULL)
	, segMax(NULL)
	, currentChar(segMin-1)
	, threadRunning(false)
	, thisThread(NULL)
{
}

PasswordGeneratorThreadWrapper::~PasswordGeneratorThreadWrapper()
{
	Finish();
	
}

void PasswordGeneratorThreadWrapper::SetSegments(string _prev,char _segMin, char _segMax)
{
	segMin = _segMin;
	segMax = _segMax <= MAXCHAR ? _segMax : MAXCHAR; // the last segment given will be smaller than the rest if the number of threads is not a divisor of the segment size
	prevString = _prev;
	currentChar = segMin -1;
}

void PasswordGeneratorThreadWrapper::Begin()
{
	threadRunning = true;
	thisThread = new thread(&PasswordGeneratorThreadWrapper::GeneratePassword, this);
}

void PasswordGeneratorThreadWrapper::Finish() // will end thread after its current cycle ends
{
	threadRunning = false;
	thisThread->join();
	delete thisThread;
}

inline bool PasswordGeneratorThreadWrapper::addOne(char& c)
{
	if (c < segMax) { // if less than the end of this thread's segment
		c++;
		return true; // we still have more to add
	}
	else {
		return false; // we have finished this segment
	}

}

void PasswordGeneratorThreadWrapper::GeneratePassword()
{
	while (threadRunning) {

		barrier->ArriveAndWait(); // wait for segment to be updated, last wait called from main thread passes barrier
		while (true)
		{
			if (addOne(currentChar)) { // not passed end of segment
				//if(threadRunning) // prevents thread from possibly getting stuck at destruction
				passwordChannel->Write(prevString + currentChar);
			}
			else {
				//if(threadRunning) // prevents thread from possibly getting stuck at destruction
					barrier->ArriveAndWait(); // wait for the rest of the threads to finish
				break; // break and wait again for seg to be updated
			}

		}
	}


}


