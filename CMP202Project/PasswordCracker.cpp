#include "PasswordCracker.h"

PasswordCracker::PasswordCracker(const int _numberOfGeneratorThreads, const int _maxChannelBufferSize)
	: targetHash(NULL)
	, maxChannelBufferSize(_maxChannelBufferSize)
	, plainTextPasswordGuessChannel(maxChannelBufferSize)
	, hashChannel(maxChannelBufferSize)
	, numberOfGeneratorThreads(_numberOfGeneratorThreads)
	, passwordTextOutChannel(1)
	, MINCHAR(' ')
	, currentPasswordRoot("")
	, generationBarrier(_numberOfGeneratorThreads+1)
	
{
	for (int i = 0; i < _numberOfGeneratorThreads; i++) {
		generatorThreads.push_back(new PasswordGeneratorThreadWrapper(&plainTextPasswordGuessChannel,&generationBarrier));
	}
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
	thread mainGenerator(&PasswordCracker::GeneratePasswordGuesses,this); // starts a thread that manages multiple generator threads


}

void PasswordCracker::SegmentPossiblePasswordGuesses() // upated the information generator threads use
{

	int numberOfCharacters = 95; // "a-z" + "A-Z" + "0-9" + " !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~" - continuous chars from ' ' to '~'

	int aproxWorkPerThread = ceil((float)numberOfCharacters / (float)numberOfGeneratorThreads);

	for (int i = 0; i < numberOfGeneratorThreads; i++) {
		generatorThreads[i]->SetSegments(currentPasswordRoot, (char)(MINCHAR + (i * aproxWorkPerThread)), (char)(MINCHAR + ((i + 1) * aproxWorkPerThread)));
	}
	
}

void PasswordCracker::GeneratePasswordGuesses() // manages generator threads 
{

	SegmentPossiblePasswordGuesses(); // initialise generator threads
	generationBarrier.ArriveAndWait(); // wait for this to finnish, then begin generation
	while (true) {
		generationBarrier.ArriveAndWait(); //  wait for generation to finnish
		SegmentPossiblePasswordGuesses(); // update generator threads
		generationBarrier.ArriveAndWait(); // wait for update to finnish, then begin generation
	}
}

void PasswordCracker::PerformHash()
{
	while(true) { // read-only in this context so no race condition
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




 