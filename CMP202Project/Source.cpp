#include "FunctionTimer.h"
#include <iostream>
#include <thread>
#include <string>

#include "Channel.h"
#include "Channel.cpp"

#include <mutex>

#include <thread>

#include <chrono>
#include <functional>


using std::lock;
using std::thread;
using std::mutex;
using std::cout;
using std::endl;
using std::hash;
using std::string;



Channel<int> chan(3);

mutex coutMutex;

//void WaitAndWrite() {
//	for (int i = 0; i < 100; i++)
//	{
//		std::this_thread::sleep_for(std::chrono::seconds(3));
//		chan.Write(i); 
//		
//	}
//}
//void PrintNum() {
//	for (int i = 0; i < 100; i++) {
//		if(i<5)
//			std::this_thread::sleep_for(std::chrono::seconds(5));
//
//		int num = chan.Read();
//
//		{
//			std::lock_guard<mutex> lock(coutMutex);
//			cout << num << endl;
//		}
//	}
//}

int main() {
	
	string a = "helo";
	hash<string> hs = hash<string>{};
	std::size_t ha = hs(a);
	int b;
	

}




