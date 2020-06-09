#pragma once
#include <string>
#include "Channel.h"

class PasswordGeneratorThreadWrapper
{
	PasswordGeneratorThreadWrapper(Channel<std::string>* _passwordChannel);
	char segMin;
	char segMax;
	
	Channel<std::string>* passwordChannel;

};

