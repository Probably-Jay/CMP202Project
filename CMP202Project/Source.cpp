#include "FunctionTimer.h"
#include <iostream>
#include <thread>


#include "Semaphore.h"

#include <mutex>

#include <thread>


using std::lock;
using std::mutex;
using std::cout;
using std::endl;

const int WIDTH = 1920;
const int HEIGHT = 1200;

// The image data.
// Each pixel is represented as 0xRRGGBB.
uint32_t image[HEIGHT][WIDTH];

void test() {
	//cout << "hello" << endl;
	int i = 1;
	int j = 2;
	float k = 0.25f * i + 0.75f + j;

	__cplusplus;
}



int main() {
	//FunctionTimer timer;
	//timer.NewTiming("test", test, 10000, true);

	//int quaterH = HEIGHT * 0.25f;
	//int quaterW = WIDTH * 0.25f;
	//for (int y = quaterH; y < quaterH+ HEIGHT / 2.f; y++) {
	//	for (int x = quaterW; x < quaterW + WIDTH / 2.f; x++) {
	//		image[y][x] = 0xFFFFFFFF;
	//	}
	//}
	//write_tga("output.tga");
	


}




// guess collection





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