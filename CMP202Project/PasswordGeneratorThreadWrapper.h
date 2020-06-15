#pragma once
#include <string>
#include <chrono>
#include "Channel.h"
#include "Barrier.h"
#include "thread"

using std::string;
using std::thread;


class PasswordGeneratorThreadWrapper
{

public:
	PasswordGeneratorThreadWrapper(Channel<std::string>* _passwordChannel,Barrier*_barrier, char _minChar, char _maxChar);

	~PasswordGeneratorThreadWrapper();
	void SetSegments(string _prev, char _segMin, char _segMax);
	void Begin();
	void Finish();

	PasswordGeneratorThreadWrapper(const PasswordGeneratorThreadWrapper&) = delete; // cannot be construction-copied 
	PasswordGeneratorThreadWrapper& operator=(const PasswordGeneratorThreadWrapper&) = delete; // cannot be copied

	inline bool addOne(char& c);
	
private:
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


	bool threadRunning;
};

