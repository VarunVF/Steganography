#include <fstream>
#include <iostream>
#include <string>

#include "Image.hpp"


int main()
{
	Image image(4);			// 4 color channels per pixel
	image.loadPNG("test/cpp_logo_input.png");

	std::string message = "Hiding in plain sight";
	image.hideLSB(message);
	image.updatePNG();		// Required before calling savePNG()
	image.savePNG("test/cpp_logo_output.png");

	// Now try getting the message back
	Image stegfile(4);
	stegfile.loadPNG("test/cpp_logo_output.png");
	std::cout
		<< "Attempt to recover secret:\n"
		<< stegfile.findLSB(message.length()) << "\n";

	std::clog << "Done.\n";
	return 0;
}
