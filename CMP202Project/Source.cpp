#include "FunctionTimer.h"
#include <iostream>
#include <thread>

using thread_local;
using std::cout;
using std::endl;

void test() {
	//cout << "hello" << endl;
	int i = 1;
	int j = 2;
	float k = 0.25f * i + 0.75f + j;
}


int main() {
	FunctionTimer timer;
	timer.NewTiming("test", test, 10000, true);
	

	parallel_for_each(


}


