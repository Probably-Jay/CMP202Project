#pragma once
#include <string>
#include <chrono>
#include "Channel.h"
#include "Barrier.h"
#include "thread"
#include "FunctionTimer.h"

using std::string;
using std::thread;


class PasswordGeneratorThreadWrapper
{
public:
	PasswordGeneratorThreadWrapper(Channel<std::string>* _passwordChannel,Barrier*_barrier, char _minChar, char _maxChar, FunctionTimer * _ft);
	~PasswordGeneratorThreadWrapper();
	void SetSegments(string _prev, char _segMin, char _segMax);
	void Begin();
	void Finish();

	PasswordGeneratorThreadWrapper(const PasswordGeneratorThreadWrapper&) = delete; // cannot be construction-copied 
	PasswordGeneratorThreadWrapper& operator=(const PasswordGeneratorThreadWrapper&) = delete; // cannot be copied
private:

	inline bool addOne(char& c);
	void GeneratePassword();
	char segMin;
	char segMax;
	
	char currentChar;
	string prevString;

	Channel<std::string>* passwordChannel;
	Barrier* barrier;

	thread* thisThread;

	char minChar;
	char maxChar;

	FunctionTimer* ft;
	TimingData* myTimingFull;
	TimingData* myTimingWork;

	bool threadRunning;
};

