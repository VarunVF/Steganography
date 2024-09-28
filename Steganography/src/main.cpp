#include <iostream>
#include <string>

#include "Image.hpp"


int main()
{
	Image image(4);			// 4 color channels per pixel
	image.loadPNG("test/cpp_logo.png");

	std::string message = "hello";
	image.hideLSB(message);
	image.updatePNG();		// Required before calling savePNG()
	image.savePNG("test/cpp_logo_output.png");

	// Now try getting the message back
	Image stegfile(4);
	stegfile.loadPNG("test/cpp_logo_output.png");
	std::cout
		<< "Attempt to recover secret:\n"
		<< stegfile.findLSB(10) << "\n";

	std::clog << "Done.\n";
	return 0;
}
