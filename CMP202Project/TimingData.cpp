#include "TimingData.h"

TimingData::TimingData(string name, void(*f)(), int ittr, bool consoleOut)
	:name(name), function(f), iterations(ittr), consoleOut(consoleOut) {
	auto timeStart = system_clock::to_time_t(system_clock::now()); // current time for file name
	filename = "Timings\\" + name + to_string(timeStart) + ".csv";

	file.open(filename);
}

TimingData::~TimingData()
{
	if (file.is_open())
		file.close();
}

void TimingData::Close()
{
	if (file.is_open())
		file.close();
}

void TimingData::RunTiming()
{
	tryOpen();
	for (int i = 0; i < iterations; i++) {
		beginTime = high_resolution_clock::now();
		callFunc(function); // inline function call to do the timed function
		endTime = high_resolution_clock::now();
		elapsedTime = duration_cast<duration<double>>(endTime - beginTime);
		timings.push_back(elapsedTime.count());
		string output = to_string(elapsedTime.count()) + ',';
		file << output;
	}
}

double TimingData::CalculateMedianTime()
{

	auto timingList = timings;

	sort(timingList.begin(), timingList.end()); // sort the list so we can find the middle value

	medianTiming = timingList[timingList.size() / (int)2]; // get the middle time

	record(to_string(medianTiming)); // record the time

	return medianTiming;

}

void TimingData::record(string data)
{
	tryOpen();
	file << '\n' + data;
}