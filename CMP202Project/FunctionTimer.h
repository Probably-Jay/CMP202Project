#pragma once

#include <string>
#include <vector>
#include <map>

#include "TimingData.h"

using std::string;
using std::vector;
using std::map;



class FunctionTimer
{
public:
	FunctionTimer();
	~FunctionTimer();

	TimingData * NewTiming(string name, void (*function)(), int iterations = 1, bool consoleOut = false); // creates new timing data object

	// used to handle the file writing for the timings of executing
	//struct TimingData {
	//	TimingData(string name, void (*f)(), int ittr, bool consoleOut):name(name),function(f),iterations(ittr),consoleOut(consoleOut) {
	//		auto timeStart = system_clock::to_time_t(system_clock::now()); // current time for file name
	//		std::string filename = "Timings\\" + name + to_string(timeStart) + ".csv";
	//		file.open(filename);
	//		RunTiming();
	//	}
	//	~TimingData() {
	//		if(file.is_open())
	//			file.close();
	//	}

	//	void RunTiming() {
	//		for (int i = 0; i < iterations; i++) {
	//			beginTime = high_resolution_clock::now();
	//			callFunc(function); // inline function call to do the timed function
	//			endTime = high_resolution_clock::now();
	//			elapsedTime = duration_cast<duration<double>>(endTime - beginTime);
	//			timings.push_back(elapsedTime.count());
	//			string output = to_string(elapsedTime.count()) + ',';
	//			file << output;
	//		}
	//	}

	//	inline void callFunc(void(*f)()){(*f)();};

	//	time_point<steady_clock> beginTime;
	//	time_point<steady_clock> endTime;
	//	duration<double> elapsedTime;
	//	string name;
	//	ofstream file;
	//	void (*function)(); // function to time
	//	int iterations;
	//	vector<double> timings;
	//	bool consoleOut;

	//};

	map<string,TimingData* > timings;
	

private:
	

};

