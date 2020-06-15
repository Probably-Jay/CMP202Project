#include "FunctionTimer.h"
#include <iostream>
#include <thread>
#include <string>

#include "PasswordCracker.h"


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


string currentPasswordRoot = "";

//void UpdatePasswordRoot()
//{
//
//
//	int len = currentPasswordRoot.length();
//
//	if (len == 0) {
//		currentPasswordRoot = MINCHAR;
//		return;
//	}
//	int end = len - 1;
//
//	for (int i = end; i >= 0; i--) { // for each letter in root, starting at far right
//		if (currentPasswordRoot[i] < MAXCHAR) {  // if letter is less than max
//
//			currentPasswordRoot = currentPasswordRoot.substr(0, i) + (char)(currentPasswordRoot[i] + 1) + currentPasswordRoot.substr(i + 1, end - i);
//
//			return;
//		}
//		else {
//			currentPasswordRoot[i] = MINCHAR; // set that letter to the start
//
//		}
//	} // we have reached the begining of the root and not yet returned
//	currentPasswordRoot = MINCHAR + currentPasswordRoot;
//
//
//
//
//}





//int main() {
//	for (size_t i = 0; i < 1; i++)
//	{
//		auto beginTime = high_resolution_clock::now();
//
//
//		PasswordCracker cracker;
//
//		string password = "~ss";
//
//		size_t hashedPassword = hash<string>{}(password);
//
//		//cout << "looking for: \"" << password << "\", hash: " << hashedPassword << endl;
//		string foundPassword = cracker.CrackPassword(hashedPassword);
//
//		auto endTime = high_resolution_clock::now();
//		auto elapsedTime = duration_cast<duration<double>>(endTime - beginTime);
//		auto time = (elapsedTime.count());
//
//
//		cout << time << endl;
//	}
//
//
//}



class fish {
public:
	void hoo() {
		cout << "hoo" << endl;
	}
};

void foo() {
	cout << "foo" << endl;
}
int bar(int g) {
	return g;
}
fish* func(char&, int, string) {
	return new fish{};
}
 
int main() {
	FunctionTimer ft;
	PasswordCracker crac;
	crac.currentPasswordRoot = "";
	
	

	int itt = 5000;
	int rep = 5000;

	
	
	
	

	//ft.RunNewTiming("hashJustWork", (FT::lambdacast<size_t,const char *[9],int,int>) [](const char ** c,  int i, int l) {size_t hashedText = hash<string>{}(c[i%l]); return hashedText; }, itt, rep,words,9,9);
	ft.RunNewTiming<PasswordCracker>("update", &PasswordCracker::UpdatePasswordRoot, &crac, itt, rep);

	
	
	

}