#include "FunctionTimer.h"

FunctionTimer::FunctionTimer()
{
	
}

FunctionTimer::~FunctionTimer()
{
}

void FunctionTimer::NewTiming(string name, void(*function)(),int iterations, bool consoleOut)
{
	timings.add(new TimingData(name, function, iterations, consoleOut));
}









