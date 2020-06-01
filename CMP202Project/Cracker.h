#pragma once
#include "Channel.h"

#include <string>
#include <mutex>
#include <thread>
#include <functional>

using std::string;
using std::lock;
using std::lock_guard;
using std::thread;
using std::mutex;
using std::hash;
using std::size_t;

#define DEFAULTMAXBUFFERSIZE 1000

class Cracker
{
public:
	// forward declarations
	struct PasswordHashPair;
	enum PasswordComplexity :int; 
	
	// public member functions
	Cracker(size_t _hash, PasswordComplexity _passwordComplexity, int _maxChannelBufferSize = DEFAULTMAXBUFFERSIZE);
	~Cracker();

	

private:
	// private member funcitons
	void GeneratePasswordGuesses();
	void PerformHash();
	void CompareHashToTarget();
	string WaitForEndOfSearch();

	// member variables
	const size_t targetHash;

	/*bool searching = false;
	mutex searchingMutex;*/

	const PasswordComplexity passwordComplexity;

	const int maxChannelBufferSize;
	Channel<string> plainTextChannel; // plaintext guesses are generated and pushed onto here
	Channel<PasswordHashPair> hashChannel; // guesses are hashed then pushed onto here
	Channel<string> passwordTextOutChannel; // used only once if the correct password is found

public:
	// contained structure and enum definitions
	struct PasswordHashPair
	{
		string password;
		size_t hash;
	};

	enum PasswordComplexityFlags {
		lowercase = 1 << 0,
		uppercase = 1 << 1,
		numbers = 1 << 2,
		symbols = 1 << 3,
	};
	
	enum PasswordComplexity :int
	{
		lowercase = PasswordComplexityFlags::lowercase,
		uppercaseLowercase = (PasswordComplexityFlags::lowercase | PasswordComplexityFlags::uppercase),
		uppercaseLowercaseNumerical = (PasswordComplexityFlags::lowercase | PasswordComplexityFlags::uppercase| PasswordComplexityFlags::numbers),
		all = (PasswordComplexityFlags::lowercase | PasswordComplexityFlags::uppercase| PasswordComplexityFlags::numbers| PasswordComplexityFlags::symbols),
	};

	 

	/*
	
		create password
		add that to chan
		hash password
		add that to chan
		compare hashes
	
	
	
	
	*/ 


};

