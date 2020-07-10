// Created by Jake Bretherton 1800231 University of abertay CMP202 coursework 2020

#include "PasswordCracker.h"

#include <iostream>
#include <thread>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::hash;
using std::string;


int main() {

	PasswordCracker passwordCracker{  }; // password cracker object
	string* password = new string{}; // the password we will be searching for

	cout << "Please enter a password to be hashed (it is reccomended to choose a password Length < 4): ";
	cin >> *password;

	auto hashedPassword = hash<string>{}(*password); // hash the password (one way operation)

	// just to prove the password cracker is not in some way cheating, delete the password
	delete password;
	password = nullptr;

	cout  << endl << "The hash of this password is " << hashedPassword << endl << " attempting to crack this hash (this may take some time)..." << endl;
	
	string output = passwordCracker.CrackPassword(hashedPassword); // crack the pasword

	cout << endl << "The found password was: \"" << output <<"\""<< endl <<endl;

	string keepOpen;
	cin >> keepOpen;

		
}




















// 
//int main() {
//	FunctionTimer ft;
//	PasswordCracker crac{  };
//
//	//char c = ' ';
//
//	string currentPasswordRoot = "";
//
//	const int itt = 50000;
//	const int rep = 50000;
//
//	crac.currentPasswordRoot = "";
//	crac.generatorThreads.push_back(new PasswordGeneratorThreadWrapper{&crac.passwordTextOutChannel,&crac.generationBarrier,' ','~'});
//
//	//ft.RunNewTiming<PasswordCracker>("genUPdWHole", &PasswordCracker::UpdatePasswordRootAndSeg, &crac, itt, rep);
//
//	//ft.RunNewTiming<void, vector<PasswordGeneratorThreadWrapper* >>("genSegWork", (FT::lambdacast<void, vector<PasswordGeneratorThreadWrapper* >>)[](vector<PasswordGeneratorThreadWrapper* > generatorThreads) {
//
//	//	int numberOfCharacters = ('~' - ' ') + 1; // "a-z" + "A-Z" + "0-9" + " !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~" - continuous chars from ' ' to '~'
//
//	//	int aproxWorkPerThread = ceil((float)numberOfCharacters / (float)1);
//
//	//	for (int i = 0; i < 1; i++) {
//	//		char max = (char)(' ' + ((i + 1) * aproxWorkPerThread));
//	//		generatorThreads[i]->SetSegments("heua", (char)(' ' + (i * aproxWorkPerThread)), max);
//	//	}
//
//
//	//	}, itt, rep, gt);
//
//
//
//	//ft.RunNewTiming<PasswordCracker,string,size_t>("whole", &PasswordCracker::CrackPassword, &crac, itt, rep, hash<string>{}("gko"));
//
//	
//	//ft.RunNewTiming<bool, char&>("genInWork", (FT::lambdacast<bool, char&>)[](char& c) {if (c < '~') { // if less than the end of this thread's segment
//	//	c++;
//	//	return true; // we still have more to add
//	//}
//	//else {
//	//	c = ' ';
//	//	return false; // we have finished this segment
//	//}},itt,rep,c);
//
//	//int curRep = 0;
//
//	//ft.RunNewTiming<void, string&>("genRootUpWork", (FT::lambdacast<void, string&>)[]( string & currentPasswordRoot)
//	//{
//	//		const int len = currentPasswordRoot.length();
//
//
//	//		for (int i = len - 1; i >= 0; i--) { // for each letter in root, starting at far right
//	//			if (currentPasswordRoot[i] < '~') {  // if letter is less than max
//	//				currentPasswordRoot = currentPasswordRoot.substr(0, i) + (char)(currentPasswordRoot[i] + 1) + currentPasswordRoot.substr(i + (int)1, len - 1 - (int)i);
//
//	//				return;
//	//			}
//	//			else {
//	//				currentPasswordRoot[i] = MINCHAR; // set that letter to the start
//
//	//			}
//	//		} // we have reached the begining of the root and not yet returned
//	//		currentPasswordRoot = MINCHAR + currentPasswordRoot;
//
//	//		/*if (count % 50000 == 0) {
//	//			currentPasswordRoot = "";
//	//		}
//	//		count++;*/
//	//	
//	//},itt,rep,currentPasswordRoot);
//
//
//
//
//	/*
//	string words[10] = { "ghdtsb","at4ew","e4g4ej","esttdj","brdbt","aggre","ghrbj","ehtdtj","dthe", "fhhtd52"};
//
//	for (int i = 0; i < itt * rep / 10; i++ ){
//		for (int j = 0; j < 10;j++) {
//			crac.plainTextPasswordGuessChannel.Write(words[j]);
//
//		}
//	}
//	*/
//	
//	//ft.RunNewTiming<PasswordCracker>("cracFromPc2", &PasswordCracker::PerformHash, &crac, itt, rep);
//
//
//	//crac.generatorThreads.push_back(new PasswordGeneratorThreadWrapper(&crac.plainTextPasswordGuessChannel, &crac.generationBarrier, ' ', '~'));
//	
//	/*ft.RunNewTiming<PasswordCracker>("upt", &PasswordCracker::UpdatePasswordRoot, &crac, itt, rep);
//	
//	ft.RunNewTiming<PasswordCracker>("seg", &PasswordCracker::SegmentPossiblePasswordGuesses, &crac, itt, rep);
//	
//	ft.RunNewTiming<PasswordGeneratorThreadWrapper, bool, char&>("thrd", &PasswordGeneratorThreadWrapper::addOne, &pw, itt, rep,c);
//	
//	ft.RunNewTiming<PasswordCracker>("whole crac", &PasswordCracker::PerformHash, &crac, itt, rep);
//	
//	ft.RunNewTiming<PasswordCracker>("whole comp", &PasswordCracker::CompareHashToTarget, &crac, itt, rep);
//	*/
//
//}

