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

	TimingData * CreateManualTiming(string name);

	template<class T_ret = void, class ... T_args>
	auto RunNewTiming(string name, T_ret(*function)(T_args ...), int iterations = 1, T_args ... otherArgs); // creates new timing data object
	
	TimingData* GetTiming(string name);
	
	void ManualTimingStart(string name, int numberOfTimings = 1) { timings[name]->ManualTimingStart(numberOfTimings); };
	void ManualTimingStop(string name, bool fast = true) { timings[name]->ManualTimingStop(fast); };
	void ManualTimingEnd(string name) { timings[name]->EndTiming(); };

																											 
	//double CalculateMedianTime(string name);
	

private:
	mutex timingsMutex;
	map<string,TimingData* > timings;

};

template<class T_ret, class ... T_args>
auto FunctionTimer::RunNewTiming(string name,  T_ret(*function)(T_args ...), int iterations, T_args ... otherArgs)
{
	TimingDataTemplate<T_ret, T_args...>* timing = new TimingDataTemplate<T_ret, T_args...>(name, function);
	{
		lock_guard<mutex> lk(timingsMutex); // raii
		timings[name] = timing;
	}
	timing->RunFunctionTiming(iterations, otherArgs ...);
	timing->EndTiming();
	return timings[name];
}