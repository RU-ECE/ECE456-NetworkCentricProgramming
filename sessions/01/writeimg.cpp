#include <cstdint>
#include <fstream>
#include <iostream>

using namespace std;

void writeImage(const char filename[], const uint32_t width, const uint32_t height) {
	ofstream imgFile(filename, ios::binary);
	if (!imgFile) {
		cerr << "Error creating file: " << filename << endl;
		return;
	}
	// note: dyhamic arrays aren't standard.
	// In modern C++, consider using std::vector or std::unique_ptr for dynamic arrays.
	// vector<char> pixels(width * height * 3);
	char pixels[width * height * 3]; // 3 bytes per pixel (RGB)

	// Initialize pixel data (for example, a gradient)
	for (uint32_t y = 0; y < height; ++y) {
		for (uint32_t x = 0; x < width; ++x) {
			pixels[(y * width + x) * 3 + 0] = static_cast<char>(x % 256); // Red
			pixels[(y * width + x) * 3 + 1] = static_cast<char>(y % 256); // Green
			pixels[(y * width + x) * 3 + 2] = 0; // Blue
		}
	}

	// Write a simple PPM image header (P6 format)
	imgFile << "P6\n" << width << " " << height << "\n255\n";
	imgFile.write(pixels, sizeof(pixels));
	imgFile.close();
}

int main() {
	writeImage("output.ppm", 256, 256);
	return 0;
}
