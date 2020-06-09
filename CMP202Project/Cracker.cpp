#include "Cracker.h"


Cracker::Cracker(size_t _hash, PasswordComplexity _passwordComplexity, int _numberOfGeneratorThreads, int _maxChannelBufferSize)
	: targetHash(_hash)
	, passwordComplexity(_passwordComplexity)
	, maxChannelBufferSize(_maxChannelBufferSize)
	, plainTextChannel(maxChannelBufferSize)
	, hashChannel(maxChannelBufferSize)
	, numberOfGeneratorThreads(_numberOfGeneratorThreads)
	, passwordTextOutChannel(1)
	, MINCHAR(' ')
	, MAXCHAR('~')
{
}

Cracker::~Cracker()
{
}

void Cracker::SegmentPossiblePasswordGuesses()
{
	while (true) {
		//int numberOfCharacters;
		//switch (passwordComplexity)
		//{
		//case Cracker::lowercase:
		//	numberOfCharacters = 26; // a-z
		//	break;
		//case Cracker::uppercaseLowercase:
		//	numberOfCharacters = 26 + 26; // A-Z
		//	break;
		//case Cracker::uppercaseLowercaseNumerical:
		//	numberOfCharacters = 26 + 26 + 10; // 0-9
		//	break;
		//case Cracker::all:
		//	numberOfCharacters = 26 + 26 + 10 + 33; // " !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~"
		//	break;
		//default:
		//	break;
		//}

		int numberOfCharacters = 95; // "a-z", "A-Z", "0-9", " !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~"

		int aproxWorkPerThread = floor((float)numberOfCharacters / (float)numberOfGeneratorThreads);

		for (int i = 0; i < numberOfGeneratorThreads; i++) {

		}

	}
}

void Cracker::GeneratePasswordGuesses()
{
	while (true) {
		/*switch (passwordComplexity)
		{
		case Cracker::lowercase:
			break;
		case Cracker::uppercaseLowercase:
			break;
		case Cracker::uppercaseLowercaseNumerical:
			break;
		case Cracker::all:
			break;
		default:
			break;
		}*/
	}
}

void Cracker::PerformHash()
{
	while(true) { // read-only in this context so no race condition
		string text = plainTextChannel.Read(); // block until new text is available, then read from it
		size_t hashedText = hash<string>{}(text); // perfom the hash using string templated hash functor struct (standard library) 
		hashChannel.Write(PasswordHashPair{ text, hashedText }); // push this to the hash channel, this operation will block if the channel is full
	}
}

void Cracker::CompareHashToTarget()
{
	while (true) {
		PasswordHashPair guess = hashChannel.Read(); // block until completed hash avaiable
		if ((guess.hash == targetHash)) {
			passwordTextOutChannel.Write(guess.password); // signal end of search
		}
	}
}

string Cracker::WaitForEndOfSearch()
{
	return passwordTextOutChannel.Read(); // block until password found (end of search)
}

inline bool Cracker::addOne(char & c)
{
	if (c < MAXCHAR) { // if less than '~'
		c++;
		return false;
	}
	else {
		c = MINCHAR; // set to ' '
		return true;
	}
	
}


 