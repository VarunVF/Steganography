#include <iostream>
#include <string>

#include "Image.hpp"


int main()
{
	Image image;
	image.loadPPM("test/input.ppm");

	std::string message = "aabcd";
	image.hideLSB(message);
	image.savePPM("test/modified.ppm");

	std::cout << image.findLSB(message.length()) << "\n";

	std::clog << "Done.\n";
	return 0;
}
