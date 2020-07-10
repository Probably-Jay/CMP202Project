#include "FunctionTimer.h"

FunctionTimer::FunctionTimer()
{
	
}

FunctionTimer::~FunctionTimer()
{
	lock_guard<mutex> lk(timingsMutex);
	timings.clear(); // this calls the destructor of each element;
}



TimingData* FunctionTimer::CreateManualTiming(string name)
{
	lock_guard<mutex> lk(timingsMutex);
	return timings[name] = new TimingData(name);
}



TimingData* FunctionTimer::GetTiming(string name)
{
	return timings[name];
}












