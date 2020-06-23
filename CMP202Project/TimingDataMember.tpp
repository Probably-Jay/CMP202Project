#include "TimingData.h"
#include "PasswordCracker.h"
#include "Channel.h"
#include <string>
#pragma once

template<class T_caller, class T_ret, class ...T_param>
TimingDataMember<T_caller, T_ret, T_param ... >::TimingDataMember(string name, T_ret(T_caller::*f)(T_param...))
	:TimingData(name), function(f)
{
}

template<class T_caller, class T_ret, class ...T_param>
void TimingDataMember<T_caller, T_ret, T_param ...>::RunFunctionTiming(const int iterations, const int repititions,	 T_caller * caller, T_param ...otherParamaters)
{
	if (consoleOut) { OutputBegin(); }
	TryOpen();
	for (int i = 0; i < iterations; i++) {
		
		
		
		for (int j = 0; j < repititions; j++) {
			//dynamic_cast<PasswordCracker>(caller)->hashChannel.Write(PasswordCracker::PasswordHashPair{ "gesfes",48646 });
			//dynamic_cast<PasswordCracker>(caller)->passwordTextOutChannel.Write("geses");
			dynamic_cast<PasswordCracker*>(caller)->hashChannel.Write(PasswordCracker::PasswordHashPair{ "gesfes",486456 });
		}

		beginTime = high_resolution_clock::now();


		for (int j = 0; j < repititions; j++) {

		CallFunc(function, caller, otherParamaters ...); // inline function call to do the timed function

		}

		endTime = high_resolution_clock::now();
		
		//dynamic_cast<PasswordCracker* > (caller)->hashChannel.Clear();

		elapsedTime = duration_cast<duration<long double>>(endTime - beginTime);
		timings.push_back(elapsedTime.count());
		string result = to_string(elapsedTime.count()) + ',';
		file << result;
		if (consoleOut) { OutputProgress(i, iterations); }
	}
	file << endl << repititions << ',' << iterations << ','<< "Repititions per timing - Number of timings" << ','<<  endl ;
	
	if (consoleOut) { OuputEnd(); }
}

