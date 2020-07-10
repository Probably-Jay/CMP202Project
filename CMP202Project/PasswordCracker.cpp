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
	generatorThreads.clear();

}


string PasswordCracker::CrackPassword(std::size_t _hash)
{
	Reset();

	targetHash = _hash;
	active = true;


	BeginThreads(waitForEndThread, &PasswordCracker::WaitForEndOfSearch); // thread will wait until password has been found
	
	// threads to do search
	BeginThreads(mainGeneratorThread, &PasswordCracker::GeneratePasswordGuesses, numberOfGeneratorThreads);
	BeginThreads(hashThreads, &PasswordCracker::PerformHash, numberOfHashingThreads);
	BeginThreads(comparisonThreads, &PasswordCracker::CompareHashToTarget, numberOfComparisonThreads);


	JoinThreads(waitForEndThread); // main thread will wait here for end of search



	// cleanup threads
	JoinThreads(mainGeneratorThread);
	JoinThreads(hashThreads);
	JoinThreads(comparisonThreads);

	


	return foundPassword;

}



void PasswordCracker::Reset()
{
	generationBarrier.Reset();
	generatorThreads.clear();
	plainTextPasswordGuessChannel.Reset();
	hashChannel.Reset();
	passwordTextOutChannel.Reset();

	currentPasswordRoot = "";
	mainGeneratorThread = nullptr;
	waitForEndThread = nullptr;
	foundPassword = "";

}

void PasswordCracker::GeneratePasswordGuesses(int _numberOfGeneratorThreads) // manages generator threads 
{

	for (int i = 0; i < _numberOfGeneratorThreads; i++) {
		generatorThreads.push_back(new PasswordGeneratorThreadWrapper(&plainTextPasswordGuessChannel, &generationBarrier, MINCHAR, MAXCHAR));
	}


	SegmentPossiblePasswordGuesses(); // initialise generator threads
	for (auto& t : generatorThreads) {
		t->Begin(); // these threads are responsible for incrementing the last character in the current guess (98.9% of the work)
	}
	generationBarrier.ArriveAndWait(); // wait for this to finnish, then begin generation

	while (active) { // main work of thread

		generationBarrier.ArriveAndWait(); //  wait for generation to finnish
		if (!active) break; // if search finished during waiting period, prevents getting stuck at next barrier
		UpdatePasswordRoot(); // progress root (all but right most character) of password
		UpdateGeneratorThreads(); // update threads with new root
		generationBarrier.ArriveAndWait(); // wait for update to finnish, then begin generation

	}

	// after password is found
	for (auto& t : generatorThreads) {
		t->Finish(); // join and cleanup for the started threads
	}
}


void PasswordCracker::SegmentPossiblePasswordGuesses() // update the information generator threads use
{
	const int numberOfCharacters = (MAXCHAR - MINCHAR) + 1; // "a-z" + "A-Z" + "0-9" + " !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~" - continuous chars from ' ' to '~'
	const int aproxWorkPerGeneratorThread = ceil((float)numberOfCharacters / (float)numberOfGeneratorThreads);

	for (int i = 0; i < numberOfGeneratorThreads; i++) {
		const char max = (char)(MINCHAR + ((i + 1) * aproxWorkPerGeneratorThread));
		const char min = (char)(MINCHAR + (i * aproxWorkPerGeneratorThread));
		generatorThreads[i]->SetSegments(min , max );
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

inline void PasswordCracker::UpdateGeneratorThreads()
{
	for (auto &t : generatorThreads) {
		t->UpdateRoot(currentPasswordRoot);
	}
}

void PasswordCracker::PerformHash()
{
	while(active) { // read-only in this context so no race condition
		string text = plainTextPasswordGuessChannel.Read(); // block until new text is available, then read from it
		size_t hashedText = hash<string>{}(text); // perfom the hash using string templated hash functor struct (standard library) 
		hashChannel.Write(PasswordHashPair{ text, hashedText }); // push this to the hash channel, this operation will block if the channel is full
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




 