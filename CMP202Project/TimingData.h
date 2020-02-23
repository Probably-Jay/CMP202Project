#pragma once

#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using std::string;
using std::ofstream;
using std::to_string;
using std::vector;
using std::map;
using std::sort;

using namespace std::chrono;

class TimingData
{
public:
	TimingData(string name, void (*f)(), int ittr, bool consoleOut);

	~TimingData();

	void Close();

	void RunTiming();

	double CalculateMedianTime();
	
	void record(string data);

	inline void tryOpen() {if (!file.is_open()) { file.open(filename);};}
	inline void callFunc(void(*f)()) { (*f)(); };

	string name;
	vector<double> timings;
	float medianTiming;

private:
	time_point<steady_clock> beginTime;
	time_point<steady_clock> endTime;
	duration<double> elapsedTime;
	string filename;
	ofstream file;
	void (*function)(); // function to time
	int iterations;
	bool consoleOut;

	

};

