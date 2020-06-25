#include "PasswordGeneratorThreadWrapper.h"


PasswordGeneratorThreadWrapper::PasswordGeneratorThreadWrapper(Channel<std::string>* _passwordChannel,Barrier*_barrier, char _minChar, char _maxChar)
	: passwordChannel(_passwordChannel)
	, minChar(_minChar)
	, maxChar(_maxChar)
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

void PasswordGeneratorThreadWrapper::SetSegments(char _segMin, char _segMax)
{
	segMin = _segMin;
	segMax = _segMax <= maxChar ? _segMax : maxChar; // the last segment given will be smaller than the rest if the number of threads is not a divisor of the segment size
	currentChar = segMin -1;
}

void PasswordGeneratorThreadWrapper::UpdateRoot(string _root)
{
	passwordRoot = _root;
	currentChar = segMin - 1;
}

void PasswordGeneratorThreadWrapper::Begin()
{
	threadRunning = true;
	thisThread = new thread(&PasswordGeneratorThreadWrapper::GeneratePassword, this);
}

void PasswordGeneratorThreadWrapper::Finish() // will end thread after its current cycle ends
{
	threadRunning = false;
	barrier->UnblockAllAndDisable(); // in case threads are stuck at barrier
	if(thisThread){
	if(thisThread->joinable())
		thisThread->join();
	delete thisThread;
	thisThread = nullptr;}
}

inline bool PasswordGeneratorThreadWrapper::addOne(char& c)
{
	if (c < segMax) { // if less than the end of this thread's segment
		c++;
		passwordChannel->Write(passwordRoot + currentChar);
		return true; // we still have more to add
	}
	else {
		passwordChannel->Write(passwordRoot + currentChar);
		c = segMin;
		return false; // we have finished this segment
	}

}



void PasswordGeneratorThreadWrapper::GeneratePassword()
{
	while (threadRunning) {

		barrier->ArriveAndWait(); // wait for segment to be updated, last wait called from main thread passes barrier
		while (threadRunning)
		{
			bool stillWithinSegment = addOne(currentChar); // add one to current char, return if still within segment

			if (stillWithinSegment) { // not passed end of segment
				passwordChannel->Write(passwordRoot + currentChar);

			}
			else {
				barrier->ArriveAndWait(); // wait for the rest of the threads to finish, signal to update root

				break; // break and wait again for seg to be updated
			}

		}
	}


}


