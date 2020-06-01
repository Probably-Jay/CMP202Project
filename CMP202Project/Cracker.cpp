#include "Cracker.h"
 

Cracker::Cracker(size_t _hash, PasswordComplexity _passwordComplexity, int _maxChannelBufferSize)
	: targetHash(_hash)
	, passwordComplexity(_passwordComplexity)
	, maxChannelBufferSize(_maxChannelBufferSize)
	, plainTextChannel(maxChannelBufferSize)
	, hashChannel(maxChannelBufferSize)
	, passwordTextOutChannel(1)
{
}

Cracker::~Cracker()
{
}

void Cracker::GeneratePasswordGuesses()
{
	while (true) {
		switch (passwordComplexity)
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
		}
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
