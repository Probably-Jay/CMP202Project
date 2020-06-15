#include "PasswordCracker.h"

PasswordCracker::PasswordCracker(const int _numberOfGeneratorThreads,
								 const int _numberOfHashingThreads,
								 const int _numberOfComparisonThreads,
								 const int _maxChannelBufferSize)
	: numberOfGeneratorThreads(_numberOfGeneratorThreads)
	, numberOfHashingThreads(_numberOfHashingThreads)
	, numberOfComparisonThreads(_numberOfComparisonThreads)
	, maxChannelBufferSize(_maxChannelBufferSize)
	, plainTextPasswordGuessChannel(maxChannelBufferSize)
	, hashChannel(maxChannelBufferSize)
	, passwordTextOutChannel(1)
	, generationBarrier(_numberOfGeneratorThreads)
	, targetHash(NULL)
	, currentPasswordRoot("")
	, active(false)
	, mainGeneratorThread(nullptr)
	, waitForEndThread(nullptr)
	, foundPassword("")
	
{
}

PasswordCracker::~PasswordCracker()
{
	for (auto& t : generatorThreads) {
		delete t;
	}
}


string PasswordCracker::CrackPassword(std::size_t _hash)
{
	targetHash = _hash;
	active = true;


	//generationMainTimingFull = functionTimer.CreateManualTiming("GenerationMain_Full_");
	////generationTimingMainWorkOnly = functionTimer.CreateManualTiming("GenerationMain_Work_");
	//
	//hashingTimingFull = functionTimer.CreateManualTiming("Hashing_Full_");
	////hashingTimingWorkOnly = functionTimer.CreateManualTiming("Hashing_Work_");
	//
	//comparisonTimingFull = functionTimer.CreateManualTiming("Comparison_Full_");
	/////comparisonTimingWorkOnly = functionTimer.CreateManualTiming("Comparison_Work_");

	BeginThreads(waitForEndThread, &PasswordCracker::WaitForEndOfSearch); // thread will wait until password has been found
	
	// threads to do search
	BeginThreads(mainGeneratorThread, &PasswordCracker::GeneratePasswordGuesses, numberOfGeneratorThreads);
	BeginThreads(hashThreads, &PasswordCracker::PerformHash, numberOfHashingThreads);
	BeginThreads(comparisonThreads, &PasswordCracker::CompareHashToTarget, numberOfComparisonThreads);


	JoinThreads(waitForEndThread); // main thread will wait here for end of search

	//generationMainTimingFull->EndTiming();
	//hashingTimingFull->EndTiming();
	//comparisonTimingFull->EndTiming();

	// cleanup threads
	JoinThreads(mainGeneratorThread);
	JoinThreads(hashThreads);
	JoinThreads(comparisonThreads);

	return foundPassword;

}



void PasswordCracker::GeneratePasswordGuesses(int _numberOfGeneratorThreads) // manages generator threads 
{


	for (int i = 0; i < _numberOfGeneratorThreads; i++) {
		generatorThreads.push_back(new PasswordGeneratorThreadWrapper(&plainTextPasswordGuessChannel, &generationBarrier, MINCHAR, MAXCHAR));
	}

	SegmentPossiblePasswordGuesses(); // initialise generator threads
	for (auto& t : generatorThreads) {
		t->Begin(); // these threads are responsible for incrementing the last character in the current guess (90% of the work)
	}
	generationBarrier.ArriveAndWait(); // wait for this to finnish, then begin generation
	
	while (active) { // main work of thread
		//generationMainTimingFull->ManualTimingStart();

		generationBarrier.ArriveAndWait(); //  wait for generation to finnish
		if (!active) break; // if search finished during waiting period, prevents getting stuck at next barrier
		UpdatePasswordRoot(); // progress root (all but right most character) of password
		SegmentPossiblePasswordGuesses(); // update generator threads
		generationBarrier.ArriveAndWait(); // wait for update to finnish, then begin generation

		//generationMainTimingFull->ManualTimingStop();
	}
	//functionTimer.ManualTimingEnd(insidefull);
	// after password is found
	for (auto& t : generatorThreads) {
		t->Finish(); // join and cleanup for the started threads
	}
}

void PasswordCracker::SegmentPossiblePasswordGuesses() // update the information generator threads use
{

	int numberOfCharacters = (MAXCHAR - MINCHAR)+1; // "a-z" + "A-Z" + "0-9" + " !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~" - continuous chars from ' ' to '~'

	int aproxWorkPerThread = ceil((float)numberOfCharacters / (float)numberOfGeneratorThreads);

	for (int i = 0; i < numberOfGeneratorThreads; i++) {
		char max = (char)(MINCHAR + ((i + 1) * aproxWorkPerThread));
		generatorThreads[i]->SetSegments(currentPasswordRoot, (char)(MINCHAR + (i * aproxWorkPerThread)), max );
	}
	
}

void PasswordCracker::UpdatePasswordRoot()
{

	
	const int len = currentPasswordRoot.length();

	

	for (int i = len-1; i >= 0; i--) { // for each letter in root, starting at far right
		if (currentPasswordRoot[i] < MAXCHAR) {  // if letter is less than max
			currentPasswordRoot = currentPasswordRoot.substr(0, i) + (char)(currentPasswordRoot[i] + 1) + currentPasswordRoot.substr(i + (int)1, len - 1  - (int)i);

			return;
		}
		else {
			currentPasswordRoot[i] = MINCHAR; // set that letter to the start

		}
	} // we have reached the begining of the root and not yet returned
	currentPasswordRoot = MINCHAR + currentPasswordRoot;

	

	



}


void PasswordCracker::PerformHash()
{
	while(active) { // read-only in this context so no race condition
		//hashingTimingFull->ManualTimingStart();

		string text = plainTextPasswordGuessChannel.Read(); // block until new text is available, then read from it
		size_t hashedText = hash<string>{}(text); // perfom the hash using string templated hash functor struct (standard library) 
		hashChannel.Write(PasswordHashPair{ text, hashedText }); // push this to the hash channel, this operation will block if the channel is full
	
		//hashingTimingFull->ManualTimingStop();

	}
}

void PasswordCracker::CompareHashToTarget()
{
	while (active) {

		PasswordHashPair guess = hashChannel.Read(); // block until completed hash avaiable
		if ((guess.hash == targetHash)) {
			passwordTextOutChannel.Write(guess.password); // signal end of search
		}


	}
}



void PasswordCracker::WaitForEndOfSearch()
{
	foundPassword = passwordTextOutChannel.Read(); // block until password found (end of search)
	EndSearch();
}

void PasswordCracker::EndSearch()
{
	active = false;
	plainTextPasswordGuessChannel.UnblockAllandDisable();
	hashChannel.UnblockAllandDisable();
	generationBarrier.UnblockAllAndDisable();
}

//void PasswordCracker::testOutput()
//{
//	unique_lock<mutex> lk(outMtx);
//	lk.unlock();
//
//	while (active) { // read-only in this context so no race condition
//		string text = plainTextPasswordGuessChannel.Read(); // block until new text is available, then read from it
//		lk.lock();
//		std::cout << text << std::endl;
//		lk.unlock();
//	}
//}

void PasswordCracker::BeginThreads(vector<thread*>& threadVector, void(PasswordCracker::* func)(void), int _numberOfThreads)
{
	for (int i = 0; i < _numberOfThreads; i++) {
		threadVector.push_back(new thread(func, this));
	}
}

void PasswordCracker::JoinThreads(thread*& _thread)
{
	if (_thread) {
		if (_thread->joinable()) {
			_thread->join();
			delete _thread;
			_thread = nullptr;
		}
	};
}

void PasswordCracker::JoinThreads(vector<thread*>& _threads)
{
	for (auto& t : _threads) {
		if (t) {
			if (t->joinable()) {
				t->join();
				delete t;
				t = nullptr;
			}
		}
	};
}




 