#include "FunctionTimer.h"
#include <iostream>
#include <thread>
#include <string>

#include "PasswordCracker.h"


#include <mutex>

#include <thread>

#include <chrono>
#include <functional>
#include <array>


using std::lock;
using std::thread;
using std::mutex;
using std::cout;
using std::endl;
using std::hash;
using std::string;
using std::array;



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


 
int main() {
	FunctionTimer ft;
	PasswordCracker crac{  };

	//char c = ' ';

	string currentPasswordRoot = "";

	const int itt = 50000;
	const int rep = 50000;

	crac.currentPasswordRoot = "";
	crac.generatorThreads.push_back(new PasswordGeneratorThreadWrapper{&crac.passwordTextOutChannel,&crac.generationBarrier,' ','~'});

	//ft.RunNewTiming<PasswordCracker>("genUPdWHole", &PasswordCracker::UpdatePasswordRootAndSeg, &crac, itt, rep);

	//ft.RunNewTiming<void, vector<PasswordGeneratorThreadWrapper* >>("genSegWork", (FT::lambdacast<void, vector<PasswordGeneratorThreadWrapper* >>)[](vector<PasswordGeneratorThreadWrapper* > generatorThreads) {

	//	int numberOfCharacters = ('~' - ' ') + 1; // "a-z" + "A-Z" + "0-9" + " !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~" - continuous chars from ' ' to '~'

	//	int aproxWorkPerThread = ceil((float)numberOfCharacters / (float)1);

	//	for (int i = 0; i < 1; i++) {
	//		char max = (char)(' ' + ((i + 1) * aproxWorkPerThread));
	//		generatorThreads[i]->SetSegments("heua", (char)(' ' + (i * aproxWorkPerThread)), max);
	//	}


	//	}, itt, rep, gt);



	//ft.RunNewTiming<PasswordCracker,string,size_t>("whole", &PasswordCracker::CrackPassword, &crac, itt, rep, hash<string>{}("gko"));

	
	//ft.RunNewTiming<bool, char&>("genInWork", (FT::lambdacast<bool, char&>)[](char& c) {if (c < '~') { // if less than the end of this thread's segment
	//	c++;
	//	return true; // we still have more to add
	//}
	//else {
	//	c = ' ';
	//	return false; // we have finished this segment
	//}},itt,rep,c);

	//int curRep = 0;

	//ft.RunNewTiming<void, string&>("genRootUpWork", (FT::lambdacast<void, string&>)[]( string & currentPasswordRoot)
	//{
	//		const int len = currentPasswordRoot.length();


	//		for (int i = len - 1; i >= 0; i--) { // for each letter in root, starting at far right
	//			if (currentPasswordRoot[i] < '~') {  // if letter is less than max
	//				currentPasswordRoot = currentPasswordRoot.substr(0, i) + (char)(currentPasswordRoot[i] + 1) + currentPasswordRoot.substr(i + (int)1, len - 1 - (int)i);

	//				return;
	//			}
	//			else {
	//				currentPasswordRoot[i] = MINCHAR; // set that letter to the start

	//			}
	//		} // we have reached the begining of the root and not yet returned
	//		currentPasswordRoot = MINCHAR + currentPasswordRoot;

	//		/*if (count % 50000 == 0) {
	//			currentPasswordRoot = "";
	//		}
	//		count++;*/
	//	
	//},itt,rep,currentPasswordRoot);




	/*
	string words[10] = { "ghdtsb","at4ew","e4g4ej","esttdj","brdbt","aggre","ghrbj","ehtdtj","dthe", "fhhtd52"};

	for (int i = 0; i < itt * rep / 10; i++ ){
		for (int j = 0; j < 10;j++) {
			crac.plainTextPasswordGuessChannel.Write(words[j]);

		}
	}
	*/
	
	//ft.RunNewTiming<PasswordCracker>("cracFromPc2", &PasswordCracker::PerformHash, &crac, itt, rep);


	//crac.generatorThreads.push_back(new PasswordGeneratorThreadWrapper(&crac.plainTextPasswordGuessChannel, &crac.generationBarrier, ' ', '~'));
	
	/*ft.RunNewTiming<PasswordCracker>("upt", &PasswordCracker::UpdatePasswordRoot, &crac, itt, rep);
	
	ft.RunNewTiming<PasswordCracker>("seg", &PasswordCracker::SegmentPossiblePasswordGuesses, &crac, itt, rep);
	
	ft.RunNewTiming<PasswordGeneratorThreadWrapper, bool, char&>("thrd", &PasswordGeneratorThreadWrapper::addOne, &pw, itt, rep,c);
	
	ft.RunNewTiming<PasswordCracker>("whole crac", &PasswordCracker::PerformHash, &crac, itt, rep);
	
	ft.RunNewTiming<PasswordCracker>("whole comp", &PasswordCracker::CompareHashToTarget, &crac, itt, rep);
	*/

}