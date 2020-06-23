#pragma once
#include "Channel.h"
#include "Barrier.h"
#include "PasswordGeneratorThreadWrapper.h"

//#include "FunctionTimer.h"

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

constexpr auto MINCHAR = ' ';
constexpr auto MAXCHAR = '~';

constexpr auto DEFAULTMAXBUFFERSIZE = 10000;
constexpr auto DEFAULNUMBEROFGENERATORTHREADS = 1;
constexpr auto DEFAULNUMBEROFHASHTHREADS = 1;
constexpr auto DEFAULNUMBEROFCOMPARISONTHREADS = 1;

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

	string CrackPassword(std::size_t hash);


	//void UpdatePasswordRootAndSeg();



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
	void WaitForEndOfSearch();
	void EndSearch();

	


	// internal thread helper functions
	void BeginThreads(thread* &threadPtr, void(PasswordCracker::* func)(void)) {threadPtr = new thread(func, this); };
	void BeginThreads(thread* &threadPtr, void(PasswordCracker::* func)(int), const int _numberOfThreads) {threadPtr = new thread(func, this, _numberOfThreads); };
	void BeginThreads(vector<thread*>& threads, void(PasswordCracker::* func)(void), int _numberOfThreads);

	void JoinThreads(thread*& _thread);
	void JoinThreads(vector<thread*>& _threads);

	// thread pointers
	thread *		mainGeneratorThread;
	vector<thread*> hashThreads;
	vector<thread*> comparisonThreads;
	thread*			waitForEndThread;

	//mutex outMtx;

	// member variables
	size_t targetHash;
	string foundPassword;

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

	

	 


};

