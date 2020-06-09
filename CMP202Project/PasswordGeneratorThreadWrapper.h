#pragma once
#include <string>
#include "Channel.h"

class PasswordGeneratorThreadWrapper
{
public:
	PasswordGeneratorThreadWrapper(Channel<std::string>* _passwordChannel);
	void SetSegments(char _segMin, char _segMax);

private:
	char segMin;
	char segMax;
	
	Channel<std::string>* passwordChannel;

};

