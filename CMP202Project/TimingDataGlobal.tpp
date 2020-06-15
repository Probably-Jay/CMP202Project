#pragma once
template<class T_ret, class ... T_param>
TimingDataGlobal<T_ret ,T_param...>::TimingDataGlobal(string name, T_ret(*f)(T_param ...))
	:TimingData(name), function(f)
{
}



template<class T_ret, class ... T_param>
void TimingDataGlobal<T_ret, T_param...>::RunFunctionTiming(const int iterations, const int repititions, T_param ... otherParamaters)
{
	if (consoleOut) { OutputBegin(); }
	TryOpen();
	for (int i = 0; i < iterations; i++) {
		beginTime = high_resolution_clock::now();


		for (int j = 0; j < repititions; j++) {

			CallFunc(function, otherParamaters ...); // inline function call to do the timed function
		}

		endTime = high_resolution_clock::now();
		elapsedTime = duration_cast<duration<double>>(endTime - beginTime);
		timings.push_back(elapsedTime.count());
		string result = to_string(elapsedTime.count()) + ',';
		file << result;
		if (consoleOut) { OutputProgress(i, iterations); }
	}
	file << endl << repititions << ',' << endl;
	if (consoleOut) { OuputEnd(); }
}