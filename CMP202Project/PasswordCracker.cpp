#include "PasswordCracker.h"

PasswordCracker::PasswordCracker(const int _numberOfGeneratorThreads,
								 const int _numberOfHashingThreads,
								 const int _numberOfComparisonThreads,
								 const int _maxChannelBufferSize)
	: numberOfGeneratorThreads(_numberOfGeneratorThreads)
	, numberOfHashingThreads(_numberOfHashingThreads)
	, numberOfComparisonThreads(_numberOfComparisonThreads)
	, targetHash(NULL)
	, maxChannelBufferSize(_maxChannelBufferSize)
	, plainTextPasswordGuessChannel(maxChannelBufferSize)
	, hashChannel(maxChannelBufferSize)
	, passwordTextOutChannel(1)
	, active(false)
	, currentPasswordRoot("")
	, generationBarrier(_numberOfGeneratorThreads)
	
{
}

PasswordCracker::~PasswordCracker()
{
	for (auto& t : generatorThreads) {
		delete t;
	}
}

void PasswordCracker::CrackPassword(std::size_t _hash)
{
	targetHash = _hash;
	active = true;

	//thread mainGenerator(&PasswordCracker::GeneratePasswordGuesses,this); // starts a thread that manages multiple generator threads
	//thread mainHash(&PasswordCracker::PerformHash, this); // starts 
	//
	
	BeginThreads(mainGeneratorThread, GeneratePasswordGuesses, (int)numberOfGeneratorThreads);
	
	





	

	

}

void PasswordCracker::GeneratePasswordGuesses(int _numberOfGeneratorThreads) // manages generator threads 
{
	for (int i = 0; i < _numberOfGeneratorThreads; i++) {
		generatorThreads.push_back(new PasswordGeneratorThreadWrapper(&plainTextPasswordGuessChannel, &generationBarrier));
	}

	SegmentPossiblePasswordGuesses(); // initialise generator threads
	for (auto& t : generatorThreads) {
		t->Begin();
	}
	generationBarrier.ArriveAndWait(); // wait for this to finnish, then begin generation
	while (active) {
		generationBarrier.ArriveAndWait(); //  wait for generation to finnish
		UpdatePasswordRoot();
		SegmentPossiblePasswordGuesses(); // update generator threads
		generationBarrier.ArriveAndWait(); // wait for update to finnish, then begin generation
	}

	// after password is found
	for (auto& t : generatorThreads) {
		t->Finish(); // cleanup for the started threads
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


	int len = currentPasswordRoot.length();
	// root is initially empty string

	int end = len - 1;
	

	for (int i = end; i >= 0; i--) { // for each letter in root, starting at far right
		if (currentPasswordRoot[i] < MAXCHAR) {  // if letter is less than max
			currentPasswordRoot = currentPasswordRoot.substr(0, i) + (char)(currentPasswordRoot[i] + 1) + currentPasswordRoot.substr(i + (int)1, end - (int)i);

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
		string text = plainTextPasswordGuessChannel.Read(); // block until new text is available, then read from it
		size_t hashedText = hash<string>{}(text); // perfom the hash using string templated hash functor struct (standard library) 
		hashChannel.Write(PasswordHashPair{ text, hashedText }); // push this to the hash channel, this operation will block if the channel is full
	}
}

void PasswordCracker::CompareHashToTarget()
{
	while (true) {
		PasswordHashPair guess = hashChannel.Read(); // block until completed hash avaiable
		if ((guess.hash == targetHash)) {
			passwordTextOutChannel.Write(guess.password); // signal end of search
		}
	}
}

string PasswordCracker::WaitForEndOfSearch()
{
	return passwordTextOutChannel.Read(); // block until password found (end of search)
}

void PasswordCracker::EndSearch()
{
	active = false;
	plainTextPasswordGuessChannel.UnblockAll();
	hashChannel.UnblockAll();
	
}

void PasswordCracker::testOutput()
{
	unique_lock<mutex> lk(outMtx);
	lk.unlock();

	while (active) { // read-only in this context so no race condition
		string text = plainTextPasswordGuessChannel.Read(); // block until new text is available, then read from it
		lk.lock();
		std::cout << text << std::endl;
		lk.unlock();
	}
}

inline void PasswordCracker::BeginThreads(vector<thread*>& threadVector, void(PasswordCracker::* func)(void), int _numberOfThreads)
{
	for (int i = 0; i < _numberOfThreads; i++) {
		threadVector.push_back(new thread(func, this));
	}
}

inline void PasswordCracker::JoinThreads(thread*& _thread)
{
	if (_thread) {
		if (_thread->joinable()) {
			_thread->join();
			delete _thread;
			_thread = nullptr;
		}
	};
}

inline void PasswordCracker::JoinThreads(vector<thread*>& _threads)
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




 