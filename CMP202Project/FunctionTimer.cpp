#include "FunctionTimer.h"

FunctionTimer::FunctionTimer()
{
	
}

FunctionTimer::~FunctionTimer()
{
	timings.clear(); // this calls the destructor of each element;
}

TimingData * FunctionTimer::NewTiming(string name, void(*function)(),int iterations, bool consoleOut)
{
	timings[name] = new TimingData(name, function, iterations, consoleOut);
	timings[name]->RunTiming();
	timings[name]->CalculateMedianTime();
	timings[name]->TryClose();
	return timings[name];
}

double FunctionTimer::CalculateMedianTime(string name)
{
	return timings[name]->CalculateMedianTime();
}










