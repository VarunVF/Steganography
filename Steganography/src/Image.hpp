#pragma once

#include <string>
#include <vector>
#include <cstdint>

#include <png.h>


class Image
{
public:
	Image();
	Image(int channelCount);
	Image(int width, int height, int channelCount = 3, std::vector<uint8_t> pixelData = {});

	// Hide a message in the image pixel data
	// using the least significant bit (LSB) of each color channel
	int hideLSB(std::string& secret);

	// Find a message from the image pixel data
	// hidden using the least significant bit (LSB) of each color channel
	std::string findLSB(size_t length);

	// Write image data to a PNG file
	int savePNG(const char* filename);

	// Read image data from a PNG file
	int loadPNG(const char* filename);

	// Write image data to a PPM file
	int savePPM(const char* filename);

	// Read image data from a PPM file
	int loadPPM(const char* filename);

	// Get the width of the image
	int width() const;

	// Get the height of the image
	int height() const;

	// Get a reference to the image pixel data
	std::vector<uint8_t>& getImageData();

private:
	int parseDimensions(std::string& text);
	int parsePixelData(std::string& text);

private:
	int m_Width;
	int m_Height;
	int m_ChannelCount;
	// Assuming range of 0..255 (1 byte) for each of 3 color channels (RGB)
	std::vector<uint8_t> m_PixelData;
	png_image m_PNG;
};
