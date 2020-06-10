#pragma once
#include <string>
#include "Channel.h"
#include "Barrier.h"
#include "thread"
constexpr auto MAXCHAR = '~';
using std::string;
using std::thread;


class PasswordGeneratorThreadWrapper
{
public:
	PasswordGeneratorThreadWrapper(Channel<std::string>* _passwordChannel,Barrier*_barrier);
	~PasswordGeneratorThreadWrapper();
	void SetSegments(string _prev, char _segMin, char _segMax);
	void Begin();
	void Finish();

private:
	//PasswordGeneratorThreadWrapper(const PasswordGeneratorThreadWrapper&) = delete; // cannot be construction-copied
	//PasswordGeneratorThreadWrapper& operator=(const PasswordGeneratorThreadWrapper&) = delete; // cannot be copied

	inline bool addOne(char& c);
	void GeneratePassword();
	char segMin;
	char segMax;
	
	char currentChar;
	string prevString;

	Channel<std::string>* passwordChannel;
	Barrier* barrier;

	thread* thisThread;

	bool threadRunning;
};

