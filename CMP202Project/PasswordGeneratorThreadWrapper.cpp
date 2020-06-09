#include "PasswordGeneratorThreadWrapper.h"

PasswordGeneratorThreadWrapper::PasswordGeneratorThreadWrapper(Channel<std::string>* _passwordChannel)
	: passwordChannel(_passwordChannel)
	, segMin(NULL)
	, segMax(NULL)
{
}

void PasswordGeneratorThreadWrapper::SetSegments(char _segMin, char _segMax)
{
	segMin = _segMin;
	segMax = _segMax;
}
