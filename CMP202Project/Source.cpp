#include "FunctionTimer.h"
#include <iostream>

using std::cout;
using std::endl;

void test() {
	cout << "hello" << endl;
}


int main() {
	FunctionTimer timer;
	timer.callFunc(test);


}


