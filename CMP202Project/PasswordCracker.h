#pragma once
#include "Channel.h"
#include "Barrier.h"
#include "PasswordGeneratorThreadWrapper.h"

#include <string>
#include <mutex>
#include <thread>
#include <functional>
#include <vector>

using std::string;
using std::lock;
using std::lock_guard;
using std::thread;
using std::mutex;
using std::hash;
using std::size_t;
using std::vector;

#define DEFAULTMAXBUFFERSIZE 1000
#define DEFAULNUMBEROFGENERATORTHREADS 1
#define DEFAULNUMBEROFHASHTHREADS 1
#define DEFAULNUMBEROFCOMPARISONTHREADS 1

class PasswordCracker
{
public:
	// forward declarations
	struct PasswordHashPair
	{
		string password;
		size_t hash;
	};
	//enum PasswordComplexity :int; 
	
	// public member functions
	PasswordCracker(const int _numberOfGeneratorThreads = DEFAULNUMBEROFGENERATORTHREADS,
		const int _numberOfHashingThreads = DEFAULNUMBEROFHASHTHREADS, 
		const int _numberOfComparisonThreads =  DEFAULNUMBEROFCOMPARISONTHREADS,
		const int _maxChannelBufferSize = DEFAULTMAXBUFFERSIZE);
	~PasswordCracker();

	void CrackPassword(std::size_t hash);

private:
	bool active;

	// private member funcitons

	// password generation
	void GeneratePasswordGuesses(int _numberOfGeneratorThreads);
	void SegmentPossiblePasswordGuesses();
	void UpdatePasswordRoot();

	// hashing
	void PerformHash();

	// hash comparision
	void CompareHashToTarget();
	
	// search managment
	string WaitForEndOfSearch();
	void EndSearch();

	// debug
	void testOutput();


	// internal thread helper functions
	inline void BeginThreads(thread* &threadPtr, void(PasswordCracker::* func)(int), const int _numberOfThreads) {threadPtr = new thread(func, this, _numberOfThreads); };

	inline void BeginThreads(vector<thread*>& threads, void(PasswordCracker::* func)(void), int _numberOfThreads);

	inline void JoinThreads(thread*& _thread);

	inline void JoinThreads(vector<thread*>& _threads);

	// thread pointers
	thread * mainGeneratorThread;
	vector<thread*> hashThreads;
	vector<thread*>  comparisonThreads;


	mutex outMtx;

	// member variables
	size_t targetHash;

	/*bool searching = false;
	mutex searchingMutex;*/

	const int numberOfGeneratorThreads;
	const int numberOfHashingThreads;
	const int numberOfComparisonThreads;

	//const PasswordComplexity passwordComplexity;

	vector<PasswordGeneratorThreadWrapper*> generatorThreads;
	Barrier generationBarrier; // for each character position wait for each thread to finish generating password attempt

	const int maxChannelBufferSize;
	Channel<string> plainTextPasswordGuessChannel; // plaintext guesses are generated and pushed onto here
	Channel<PasswordHashPair> hashChannel; // guesses are hashed then pushed onto here
	Channel<string> passwordTextOutChannel; // used only once if the correct password is found

	string currentPasswordRoot;

	// allowed characters for password generation are the continuous block of characters from ' '(32) to '~'(126)
	//const char MINCHAR; // minimum character that can be used in password guess generation, ' '
	//const char MAXCHAR; // maximim, '~'

public:
	// contained structure and enum definitions


	//enum PasswordComplexityFlags {
	//	lowercase = 1 << 0,
	//	uppercase = 1 << 1,
	//	numbers = 1 << 2,
	//	symbols = 1 << 3,
	//};
	//
	//enum PasswordComplexity :int
	//{
	//	lowercase = PasswordComplexityFlags::lowercase,
	//	uppercaseLowercase = (PasswordComplexityFlags::lowercase | PasswordComplexityFlags::uppercase),
	//	uppercaseLowercaseNumerical = (PasswordComplexityFlags::lowercase | PasswordComplexityFlags::uppercase| PasswordComplexityFlags::numbers),
	//	all = (PasswordComplexityFlags::lowercase | PasswordComplexityFlags::uppercase| PasswordComplexityFlags::numbers| PasswordComplexityFlags::symbols),
	//};

	 

	/*
	
		create password
		add that to chan
		hash password
		add that to chan
		compare hashes
	
	
	
	
	*/ 


};

