#include <fstream>
#include <iostream>
#include <string>

#include "Image.hpp"


int main()
{
	// Input image, 4 color channels per pixel
	Image image(4);
	image.loadPNG("test/cpp_logo_input.png");

	std::string message = "Hiding in plain sight";
	image.hideLSB(message);
	std::cout << "Secret hidden.\n";
	image.savePNG("test/cpp_logo_output.png");


	// Now try getting the message back
	Image stegfile(4);
	stegfile.loadPNG("test/cpp_logo_output.png");

	std::cout << "Attempt to recover secret:\n";
	std::string secret = stegfile.findLSB(message.length());
	std::cout << secret << "\n";


	std::clog << "Done.\n";
	std::cin.get();
	return 0;
}
