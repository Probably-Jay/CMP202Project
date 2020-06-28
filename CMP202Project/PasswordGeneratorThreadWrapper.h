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
	PasswordGeneratorThreadWrapper(Channel<std::string>* _passwordChannel, Barrier*_barrier, char _minChar, char _maxChar);

	~PasswordGeneratorThreadWrapper();
	void SetSegments(char _segMin, char _segMax); // delagate work to this thread
	void UpdateRoot(string _root); // thread work
	void Begin(); // begin work
	void Finish(); // end and clean up

	PasswordGeneratorThreadWrapper(const PasswordGeneratorThreadWrapper&) = delete; // cannot be construction-copied 
	PasswordGeneratorThreadWrapper& operator=(const PasswordGeneratorThreadWrapper&) = delete; // cannot be copied

	inline bool addOne(char& c);
	
private:
	void GeneratePassword();
	char segMin;
	char segMax;
	
	char currentChar;
	string passwordRoot;

	Channel<std::string>* passwordChannel;
	Barrier* barrier;

	thread* thisThread;

	char minChar;
	char maxChar;


	bool threadRunning;
};

