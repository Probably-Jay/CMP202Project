#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "TimingData.h"

using std::string;
using std::vector;
using std::map;


// class that times functions and stores the timings as TimingData classes
class FunctionTimer
{
public:
	FunctionTimer();
	~FunctionTimer();

	TimingData * NewTiming(string name, void (*function)(), int iterations = 1, bool consoleOut = false); // creates new timing data object
	double CalculateMedianTime(string name);
	map<string,TimingData* > timings;
	

private:
	

};

