#pragma once
#include <string>
#include "Channel.h"
#include "Barrier.h"
constexpr auto MAXCHAR = '~';
using std::string;


class PasswordGeneratorThreadWrapper
{
public:
	PasswordGeneratorThreadWrapper(Channel<std::string>* _passwordChannel,Barrier*_barrier);
	void SetSegments(string _prev, char _segMin, char _segMax);
	inline bool addOne(char& c);
	void GeneratePassword();

private:
	char segMin;
	char segMax;
	
	char currentChar;
	string prevString;

	Channel<std::string>* passwordChannel;
	Barrier* barrier;
};

