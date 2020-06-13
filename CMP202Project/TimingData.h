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
class TimingData
{
	friend class FunctionTimer;
public:
	TimingData(string name);

	~TimingData();


	void ManualTimingStart();
	void ManualTimingStop(bool fast = true);

	void EndTiming();

	string GetName() { return name; };
	
private:

	TimingData(string name, void (*f)(void), int ittr, bool consoleOut);
	string name;
	void RunFunctionTiming();
	double CalculateMedianTime();
	
	void Record(string data);

	void OutputProgress(float progress, float total);
	void OuputEnd();
	void OutputBegin();

	inline void TryOpen() {if (!file.is_open()) { file.open(filename);};}

	inline void TryClose() {if (file.is_open()) { file.close();};}

	//inline void TryClose() {file.is_open() ? file.close() : void();}

	inline void CallFunc(void(*f)()){(*f)();}; // calls function pointer

	

	vector<double> timings;
	float medianTiming;


	time_point<steady_clock> beginTime;
	time_point<steady_clock> endTime;
	duration<double> elapsedTime;
	string filename;
	ofstream file;
	void (*function)(); // function to time
	int iterations;
	bool consoleOut;

	mutex timeMutex;
	mutex fileWriteMutex;
	unique_lock<mutex> timeLock;
	

};

