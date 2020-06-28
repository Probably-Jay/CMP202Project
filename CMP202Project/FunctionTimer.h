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

namespace FT {
	// templated cast from lambda to function pointer, avoids writing `(void(*)())[](){//code}`
	template<class T_ret = void, class ... T_arg>
	using lambdacast = T_ret(*)(T_arg ...); 
}

// class that times functions and stores the timings as TimingData classes
class FunctionTimer
{
public:
	FunctionTimer();
	~FunctionTimer();

	TimingData * CreateManualTiming(string name);

	

	template<class T_ret = void, class ... T_args> // timing global function
	auto RunNewTiming(string name, T_ret(*function)(T_args ...), const int iterations = 1, const int repititions = 1, T_args ... otherArgs); // creates new timing data object
	
 	template<class T_caller, class T_ret = void, class ... T_args > // timing a public member function
	auto RunNewTiming(string name, T_ret(T_caller::*function)(T_args ...), T_caller*, const int iterations = 1, const int repititions = 1, T_args ... otherArgs); // creates new timing d



	TimingData* GetTiming(string name);
	
	void ManualTimingStart(string name, int numberOfTimings = 1) { timings[name]->ManualTimingStart(numberOfTimings); };
	void ManualTimingStop(string name, bool fast = true) { timings[name]->ManualTimingStop(fast); };
	void ManualTimingEnd(string name) { timings[name]->EndTiming(); };

																											 

private:
	mutex timingsMutex;
	map<string,TimingData* > timings;

};

// timing for global/anonymous function
template<class T_ret, class ... T_args>
auto FunctionTimer::RunNewTiming(string name,  T_ret(*function)(T_args ...), const int iterations, const int repititions, T_args ... otherArgs)
{
	TimingDataGlobal<T_ret, T_args...>* timing = new TimingDataGlobal<T_ret, T_args...>(name, function);
	{
		lock_guard<mutex> lk(timingsMutex); // raii
		timings[name] = timing;
	}
	timing->RunFunctionTiming(iterations,repititions, otherArgs ...);
	timing->EndTiming();
	return timings[name];
}

// timing for member function
template<class T_caller, class T_ret, class ...T_args>
auto FunctionTimer::RunNewTiming(string name, T_ret(T_caller::*function)(T_args...), T_caller* caller, const int iterations, const int repititions, T_args ...otherArgs)
{
	TimingDataMember<T_caller,T_ret, T_args...>* timing = new TimingDataMember<T_caller, T_ret, T_args...>(name, function);
	{
		lock_guard<mutex> lk(timingsMutex); // raii
		timings[name] = timing;
	}
	timing->RunFunctionTiming(iterations, repititions, caller, otherArgs ...);
	timing->EndTiming();
	return timings[name];
	
}
