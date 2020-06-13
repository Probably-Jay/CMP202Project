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

TimingData * FunctionTimer::RunNewTiming(string name, void(*function)(),int iterations, bool consoleOut)
{
	{
		lock_guard<mutex> lk(timingsMutex); // raii
		timings[name] = new TimingData(name, function, iterations, consoleOut);
	}
	timings[name]->RunFunctionTiming();
	timings[name]->EndTiming();
	return timings[name];
}

TimingData* FunctionTimer::GetTiming(string name)
{
	return timings[name];
}












