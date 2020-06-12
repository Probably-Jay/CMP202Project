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

void UpdatePasswordRoot()
{


	int len = currentPasswordRoot.length();

	if (len == 0) {
		currentPasswordRoot = MINCHAR;
		return;
	}
	int end = len - 1;

	for (int i = end; i >= 0; i--) { // for each letter in root, starting at far right
		if (currentPasswordRoot[i] < MAXCHAR) {  // if letter is less than max

			currentPasswordRoot = currentPasswordRoot.substr(0, i) + (char)(currentPasswordRoot[i] + 1) + currentPasswordRoot.substr(i + 1, end - i);

			return;
		}
		else {
			currentPasswordRoot[i] = MINCHAR; // set that letter to the start

		}
	} // we have reached the begining of the root and not yet returned
	currentPasswordRoot = MINCHAR + currentPasswordRoot;




}



int main() {
	for (size_t i = 0; i < 1; i++)
	 {
		PasswordCracker cracker;

		string password = "pass";

		size_t hashedPassword = hash<string>{}(password);

		//cout << "looking for: \"" << password << "\", hash: " << hashedPassword << endl;
		string foundPassword = cracker.CrackPassword(hashedPassword);

		//cout << "found : " << foundPassword << endl;
	}


}