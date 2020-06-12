#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <mutex>

#include "TimingData.h"

using std::string;
using std::vector;
using std::map;
using std::mutex;
using std::lock_guard;

// class that times functions and stores the timings as TimingData classes
class FunctionTimer
{
public:
	FunctionTimer();
	~FunctionTimer();

	TimingData* CreateManualTiming(string name);
	TimingData* RunNewTiming(string name, void (*function)(), int iterations = 1, bool consoleOut = false); // creates new timing data object
	
	void ManualTimingBegin(string name) { timings[name]->ManualTimingStart(); };
	void ManualTimingStop(string name) { timings[name]->ManualTimingStop(); };
	void ManualTimingEnd(string name) { timings[name]->EndTiming(); };

																											 
	//double CalculateMedianTime(string name);
	map<string,TimingData* > timings;
	

private:
	mutex timingsMutex;

};

