#include "TimingData.h"

TimingData::TimingData(string name, void(*f)(), int ittr, bool consoleOut)
	:name(name), function(f), iterations(ittr), consoleOut(consoleOut)
{
	auto timeStart = system_clock::to_time_t(system_clock::now()); // current time to create unique file name (ensures no accidental overwite)
	filename = "Timings\\" + name + to_string(timeStart) + ".csv";

	file.open(filename);
}

TimingData::~TimingData()
{
	TryClose();
}



void TimingData::RunTiming()
{
	if (consoleOut) { OutputBegin(); }
	TryOpen();
	for (int i = 0; i < iterations; i++) {
		beginTime = high_resolution_clock::now();
		CallFunc(function); // inline function call to do the timed function
		endTime = high_resolution_clock::now();
		elapsedTime = duration_cast<duration<double>>(endTime - beginTime);
		timings.push_back(elapsedTime.count());
		string output = to_string(elapsedTime.count()) + ',';
		file << output;
		if (consoleOut) { OutputProgress(i,iterations); }
	}
	file << endl;
	if (consoleOut) { OuputEnd(); }
}

double TimingData::CalculateMedianTime()
{

	auto timingList = timings;

	sort(timingList.begin(), timingList.end()); // sort the list so we can find the middle value

	medianTiming = timingList[timingList.size() / (int)2]; // get the middle time

	Record(to_string(medianTiming)); // record the time

	return medianTiming;

}

void TimingData::Record(string data)
{
	TryOpen();
	file << data << endl;
}

void TimingData::OutputProgress(float progress, float total)
{
	float percent = 100.f * (progress / total);
	string outputPercent = to_string(percent);
	outputPercent = outputPercent.substr(0, 5);
	cout << "\b";cout << "\b";cout << "\b";cout << "\b";cout << "\b";cout << "\b"; // XX.XX%
	cout << outputPercent << "%";

}

void TimingData::OuputEnd()
{
	cout << "\b"; cout << "\b"; cout << "\b"; cout << "\b"; cout << "\b"; cout << "\b";  // XX.XX%
	cout << "100.00%" << endl;	
}

void TimingData::OutputBegin()
{
	cout << "Timing for: " << name << endl;
}
