#pragma once

#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

#include <mutex>

using std::string;
using std::ofstream;
using std::to_string;
using std::vector;
using std::map;
using std::sort;
using std::cout;
using std::endl;
using std::mutex;
using std::unique_lock;
using std::lock_guard;

using namespace std::chrono;

// class for handing timing on one function and storing that timing in a file

class TimingData{
public:
	TimingData(string name);
	~TimingData();

	void ManualTimingStart(int numberOfTimings = 1);
	void ManualTimingStop(bool fast = true);

	void EndTiming();

	string GetName() { return name; };
	
protected:
	
	string name;
	double CalculateMedianTime();
	
	void Record(string data);

	void OutputProgress(float progress, float total);
	void OuputEnd();
	void OutputBegin();

	inline void TryOpen() {if (!file.is_open()) { file.open(filename);};}

	inline void TryClose() {if (file.is_open()) { file.close();};}

	//inline void TryClose() {file.is_open() ? file.close() : void();}


	

	vector<double> timings;
	float medianTiming;


	time_point<steady_clock> beginTime;
	time_point<steady_clock> endTime;
	duration<double> elapsedTime;
	string filename;
	ofstream file;
	bool consoleOut;

	mutex timeMutex;
	mutex fileWriteMutex;
	unique_lock<mutex> timeLock;

	int timingCount;
	string timingResult;
	
};

template<class T_ret, class ... T_param>
class TimingDataTemplate : TimingData
{
	friend class FunctionTimer;
public:

	TimingDataTemplate(string name, T_ret(*f)(T_param ...));
	void RunFunctionTiming(int itterations, T_param ... otherParamaters);

private:

	T_ret(*function)(T_param ...); // function that will be timed

	inline void CallFunc(T_ret(*func)(T_param ...),T_param ... params) { (*func)(params ...); }; // calls function pointer




};

template<class T_ret, class ... T_param>
TimingDataTemplate<T_ret ,T_param...>::TimingDataTemplate(string name, T_ret(*f)(T_param ...))
	:TimingData(name), function(f)
{
}



template<class T_ret, class ... T_param>
void TimingDataTemplate<T_ret, T_param...>::RunFunctionTiming(int iterations, T_param ... otherParamaters)
{
	if (consoleOut) { OutputBegin(); }
	TryOpen();
	for (int i = 0; i < iterations; i++) {
		beginTime = high_resolution_clock::now();
		CallFunc(function, otherParamaters ...); // inline function call to do the timed function
		endTime = high_resolution_clock::now();
		elapsedTime = duration_cast<duration<double>>(endTime - beginTime);
		timings.push_back(elapsedTime.count());
		string result = to_string(elapsedTime.count()) + ',';
		file << result;
		if (consoleOut) { OutputProgress(i, iterations); }
	}
	file << endl;
	if (consoleOut) { OuputEnd(); }
}