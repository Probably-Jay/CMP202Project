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


	time_point<high_resolution_clock> beginTime;
	time_point<high_resolution_clock> endTime;
	duration<long double> elapsedTime;
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
class TimingDataGlobal : TimingData
{
	friend class FunctionTimer;
public:

	TimingDataGlobal(string name, T_ret(*f)(T_param ...));
	void RunFunctionTiming(const int itterations, const int repititions, T_param ... otherParamaters);

private:

	T_ret(*function)(T_param ...); // function that will be timed

	inline void CallFunc(T_ret(*func)(T_param ...),T_param ... params) { (*func)(params ...); }; // calls function pointer




};

template<class T_caller, class T_ret, class ... T_param>
class TimingDataMember : TimingData
{
	friend class FunctionTimer;
public:

	TimingDataMember(string name, T_ret(T_caller::*f)(T_param ...));
	void RunFunctionTiming(const int itterations, const int repititions, T_caller * caller, T_param ... otherParamaters);

private:

	T_ret(T_caller::*function)(T_param ...); // function that will be timed

	inline void CallFunc(T_ret(T_caller::*func)(T_param ...), T_caller * caller,T_param ... params) { (caller->*func)(params ...); }; // calls function pointer




};

#include "TimingDataGlobal.tpp"
#include "TimingDataMember.tpp"

