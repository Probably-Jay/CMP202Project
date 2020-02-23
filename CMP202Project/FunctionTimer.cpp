#include "FunctionTimer.h"

FunctionTimer::FunctionTimer()
{
	
}

FunctionTimer::~FunctionTimer()
{
	timings.clear(); // this calls the destructor of each element;
}

TimingData *  FunctionTimer::NewTiming(string name, void(*function)(),int iterations, bool consoleOut)
{
	timings[name] = new TimingData(name, function, iterations, consoleOut);
	return timings[name];
}









