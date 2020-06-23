#include "FunctionTimer.h"
#include <iostream>
#include <thread>

using std::cout;
using std::endl;
using std::vector;
std::map<char, char> character = {
	{NULL,'a'},
	{'a','b'},
	{'b','c'},
	{'c','d'},
	{'d','e'},
	{'e','f'},
	{'f','g'},
	{'g','h'},
	{'h','i'},
	{'i','j'},
	{'j','k'},
	{'k','l'},
	{'l','m'},
	{'m','n'},
	{'n','o'},
	{'o','p'},
	{'p','q'},
	{'q','r'},
	{'r','t'},
	{'s','s'},
	{'t','v'},
	{'u','u'},
	{'v','w'},
	{'w','x'},
	{'x','y'},
	{'y','z'},
	{'z','A'},
	{'A','B'},
	{'B','C'},
	{'C','D'},
	{'D','E'},
	{'E','F'},
	{'F','G'},
	{'G','H'},
	{'H','I'},
	{'I','J'},
	{'J','K'},
	{'K','L'},
	{'L','M'},
	{'M','N'},
	{'N','O'},
	{'O','P'},
	{'P','Q'},
	{'Q','R'},
	{'R','S'},
	{'S','T'},
	{'T','U'},
	{'U','V'},
	{'V','W'},
	{'W','X'},
	{'X','Y'},
	{'Y','Z'},
	{'Z','0'},
	{'0','1'},
	{'1','2'},
	{'2','3'},
	{'3','4'},
	{'4','5'},
	{'5','6'},
	{'6','7'},
	{'7','8'},
	{'8','9'},
	{'9','"'},
	{'"','#'},
	{'#','$'},
	{'$','&'},
	{'&','\''},
	{'\'','\\'},
	{'\\','('},
	{'(',')'},
	{')','*'},
	{'*','+'},
	{'+',','},
	{',','-'},
	{'-','.'},
	{'.','/'},
	{'/',':'},
	{':',';'},
	{';','<'},
	{'<','='},
	{'=','>'},
	{'>','?'},
	{'?','@'},
	{'@','['},
	{'[',']'},
	{']','^'},
	{'^','_'},
	{'_','`'},
	{'`','{'},
	{'{','|'},
	{'|','}'},
	{'}','~'},
	{'~',NULL}/*,
	{' ','£'},
	{'£',NULL},*/
};

#define MAX_ATTEMPTS 1000000
#define MAX_LENGTH 20


string PASSWORD;
int passwordLength;

vector<vector<char>> characterRange;
vector<char> lowercase = { 'a','z' };
vector<char> upercase = { 'A','Z'};
vector<char> numbers = { '0','9' };
vector<char> all_including_special = { ' ','~' };
int CURRENTINDEX = 0;
const bool DEBUG = false;

void AddOne(string& s, int offset = 0) {
	
	int CURRENTCHARTYPE = 0;
	for (int i = 0; i < characterRange.size(); i++) {
		if (s[offset] >= characterRange[i][0] && s[offset] <= characterRange[i][1]) {
			CURRENTCHARTYPE = i;
		}
	}
	if (offset == s.size()) {
		s.push_back(characterRange[0][0]);
		return;
	}
	char n = ++s[offset];
	if (n <= characterRange[CURRENTCHARTYPE][1]) {
		s[offset] = n;
		return;
	}
	else {
		if (CURRENTCHARTYPE < characterRange.size() - 1) {
			CURRENTCHARTYPE++;
			s[offset] = characterRange[CURRENTCHARTYPE][0];
		}
		else {
			CURRENTCHARTYPE = 0;
			s[offset] = characterRange[0][0];
			AddOne(s, ++offset);
		}
	}
}



void BruteForce() {
	string guess = "";
	guess += characterRange[0][0];
		while (guess != PASSWORD)
		{  
			AddOne(guess);
		}
		if (guess == PASSWORD) {
			
			return;
		}
}


int main() {
	srand((unsigned)time(NULL));
	FunctionTimer timer;
	PASSWORD = "";
	for (int i = 1; i < 6; i++) {
	
		PASSWORD += "j";
		int itterations;
		itterations = 10;

		cout << endl << "Lenght: " + to_string(i) << " password " << PASSWORD << " itterations " << to_string(itterations)<< endl;

		
		characterRange.clear();

		characterRange.push_back(lowercase);
		cout<< "lowercase " + to_string(i) << endl;
		timer.NewTiming("lowerCase-"+to_string(i)+'_', BruteForce, itterations);
		

		cout << "uppercase " + to_string(i) << endl;
		characterRange.push_back(upercase);
		timer.NewTiming("upperCase-" + to_string(i) + '_', BruteForce, itterations);

		cout << "numerical " + to_string(i) << endl;
		characterRange.push_back(numbers);
		timer.NewTiming("numbers-" + to_string(i) + '_', BruteForce, itterations);
	
		cout << "special " + to_string(i) << endl;
		characterRange.clear();
		characterRange.push_back(all_including_special);
		timer.NewTiming("spec-" + to_string(i) + '_', BruteForce, itterations);
	

		characterRange.clear();
	}

}






















const int WIDTH = 1920;
const int HEIGHT = 1200;
//
//// The image data.
//// Each pixel is represented as 0xRRGGBB.
//uint32_t image[HEIGHT][WIDTH];
//
//void test() {
//	//cout << "hello" << endl;
//	int i = 1;
//	int j = 2;
//	float k = 0.25f * i + 0.75f + j;
//}
//
//
//
//void write_tga(const char* filename)
//{
//	ofstream outfile(filename, ofstream::binary);
//
//	uint8_t header[18] = {
//		0, // no image ID
//		0, // no colour map
//		2, // uncompressed 24-bit image
//		0, 0, 0, 0, 0, // empty colour map specification
//		0, 0, // X origin
//		0, 0, // Y origin
//		WIDTH & 0xFF, (WIDTH >> 8) & 0xFF, // width
//		HEIGHT & 0xFF, (HEIGHT >> 8) & 0xFF, // height
//		24, // bits per pixel
//		0, // image descriptor
//	};
//	outfile.write((const char*)header, 18);
//
//	for (int y = 0; y < HEIGHT; ++y)
//	{
//		for (int x = 0; x < WIDTH; ++x)
//		{
//			uint8_t pixel[3] = {
//				image[y][x] & 0xFF, // blue channel
//				(image[y][x] >> 8) & 0xFF, // green channel
//				(image[y][x] >> 16) & 0xFF, // red channel
//			};
//			outfile.write((const char*)pixel, 3);
//		}
//	}
//
//	outfile.close();
//	if (!outfile)
//	{
//		// An error has occurred at some point since we opened the file.
//		cout << "Error writing to " << filename << endl;
//		exit(1);
//	}
//}

//int main() {
//	//FunctionTimer timer;
//	//timer.NewTiming("test", test, 10000, true);
//
//
//	//for (int y = HEIGHT/2.f; y >0 ; y--) {
//	for (int y = HEIGHT/2.f; y >0 ; y--) {
//		for (int x = 0; x < WIDTH /2.f; x++) {
//			image[y][x] = 0xFFFFFFFF;
//		}
//	}
//	write_tga("output.tga");
//	
//
//
//}



