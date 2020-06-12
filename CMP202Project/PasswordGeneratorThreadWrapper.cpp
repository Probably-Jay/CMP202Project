#include "PasswordGeneratorThreadWrapper.h"


PasswordGeneratorThreadWrapper::PasswordGeneratorThreadWrapper(Channel<std::string>* _passwordChannel,Barrier*_barrier, char _minChar, char _maxChar, FunctionTimer * _ft)
	: passwordChannel(_passwordChannel)
	, minChar(_minChar)
	, maxChar(_maxChar)
	, barrier(_barrier)
	, segMin(NULL)
	, segMax(NULL)
	, currentChar(segMin-1)
	, threadRunning(false)
	, thisThread(NULL)
	, ft(_ft)
{
	myTimingFull = ft->CreateManualTiming("GenerationInside_Full_");
	myTimingWork = ft->CreateManualTiming("GenerationMain_Work_"); // move outside
}

PasswordGeneratorThreadWrapper::~PasswordGeneratorThreadWrapper()
{
	Finish();
	
}

void PasswordGeneratorThreadWrapper::SetSegments(string _prev,char _segMin, char _segMax)
{
	segMin = _segMin;
	segMax = _segMax <= maxChar ? _segMax : maxChar; // the last segment given will be smaller than the rest if the number of threads is not a divisor of the segment size
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
	barrier->UnblockAll(); // in case threads are stuck at barrier
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
		while (threadRunning)
		{
			bool stillWithinSegment = addOne(currentChar);

			if (stillWithinSegment) { // not passed end of segment
				//if(threadRunning) // prevents thread from possibly getting stuck at destruction
				passwordChannel->Write(prevString + currentChar);
				//std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			else {
				//if(threadRunning) // prevents thread from possibly getting stuck at destruction
					barrier->ArriveAndWait(); // wait for the rest of the threads to finish
				break; // break and wait again for seg to be updated
			}

		}
	}


}


