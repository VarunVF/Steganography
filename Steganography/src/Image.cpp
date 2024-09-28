#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>

#include <png.h>
#include <zlib.h>

#include "Image.hpp"
#include "parseOneInteger.hpp"


// The control structure used by libpng
static png_image image;
static png_bytep buffer = NULL;


Image::Image()
	: m_Width(0), m_Height(0), m_ChannelCount(3), m_PixelData()
{
}

Image::Image(int channelCount)
	: m_Width(0), m_Height(0), m_ChannelCount(channelCount), m_PixelData()
{
}

Image::Image(int width, int height, int channelCount, std::vector<uint8_t> pixelData)
	: m_Width(width), m_Height(height), m_ChannelCount(channelCount), m_PixelData(pixelData)
{
}

int Image::hideLSB(std::string& secret)
{
	// Check if the image is large enough to hide this message
	size_t requiredSize = secret.length() * 8;
	size_t requiredPixels = (size_t) std::ceil( (float)requiredSize / m_ChannelCount );

	size_t actualSize = m_PixelData.size();
	size_t actualPixels = (size_t) std::ceil( (float)actualSize / m_ChannelCount );

	if (actualSize < requiredSize)
	{
		std::cerr
			<< "Image is too small to store this secret:\n"
			<< "Requires at least " << requiredPixels << " pixels but only has " << actualPixels << ".\n";
		return 1;
	}

	size_t secret_idx = 0;
	for (size_t i = 0; i < 8 * secret.length(); i += 8, secret_idx++)
	{
		for (uint8_t bit = 0; bit < 8; bit++)
		{
			if (secret[secret_idx] & (1 << bit))
				m_PixelData[i + bit] |= 1;				// Set LSB
			else
				m_PixelData[i + bit] &= 0b11111110;		// Reset LSB
		}
	}

	std::clog << "Secret hidden.\n";
	return 0;
}

std::string Image::findLSB(size_t secretLength)
{
	std::string secret;
	secret.reserve(secretLength);

	// Validate length
	if (secretLength * 8 > m_PixelData.size())
	{
		std::cerr << "Length of secret is out of bounds\n";
		exit(1);
	}

	for (size_t i = 0; i < 8 * secretLength; i += 8)
	{
		// Build up a char of the secret
		char ch = 0;
		for (uint8_t bit = 0; bit < 8; bit++)
			if (m_PixelData[i + bit] & 1)		// test if LSB is set
				ch |= 1 << bit;

		secret += ch;
	}

	return secret;
}

int Image::loadPNG(const char* filename)
{
	// Verify pixel format is RGBA
	if (m_ChannelCount != 4)
	{
		std::cerr
			<< "Image is not in expected RGBA format for saving as PNG:\n"
			<< "Requires 4 channels per pixel but has " << m_ChannelCount << ".\n";
		return 1;
	}

	// Using the libpng usage example:
	// https://github.com/pnggroup/libpng/blob/libpng16/example.c

	// Initialise the png_image structure
	memset(&image, 0, (sizeof image));
	image.version = PNG_IMAGE_VERSION;

	if (png_image_begin_read_from_file(&image, filename))
	{
		// Set the format in which to read the PNG file
		image.format = PNG_FORMAT_RGBA;

		// Allocate memory to hold image in this format
		buffer = (png_bytep)malloc(PNG_IMAGE_SIZE(image));

		if (buffer != NULL &&
			png_image_finish_read(&image, NULL, buffer, 0, NULL) != 0)
		{
			// Buffer is valid:
			// Copy the image pixels into m_PixelData.

			auto imageChannelCount = 4 * image.width * image.height;
			for (unsigned int i = 0; i < imageChannelCount; i++)
				m_PixelData.push_back(buffer[i]);

			return 0;
		}
		else
		{
			// In this case, if there wasn't enough memory for 'buffer',
			// we didn't complete the read, so we must free the image:

			if (buffer == NULL)
				png_image_free(&image);
			else
				free(buffer);
		}
	}

	// Something went wrong while reading the file.
	// libpng stores a textual message in the 'png_image' structure:
	std::cerr << "Error: " << image.message << "\n";
	return 1;
}

int Image::updatePNG()
{
	// Update the PNG buffer using m_PixelData

	auto imageChannelCount = 4 * image.width * image.height;
	if (m_PixelData.size() < imageChannelCount)
	{
		std::cerr
			<< "updatePNG failed:\n"
			<< "Size of pixel data and of PNG buffer do not match\n";
		return 1;
	}

	for (unsigned int i = 0; i < imageChannelCount; i++)
		buffer[i] = m_PixelData[i];
	
	return 0;
}

int Image::savePNG(const char* filename)
{
	// Check that the buffer is valid
	if (buffer == NULL)
		return 1;

	if (png_image_write_to_file(&image, filename, 0, buffer, 0, NULL) == 0)
	{
		// The buffer was not written successfully
		return 1;
	}

	return 0;
}

int Image::parseDimensions(std::string& text)
{
	if (parseOneInteger(text, m_Width) != 0
	 || parseOneInteger(text, m_Height) != 0)
		return 1;
	return 0;
}

int Image::parsePixelData(std::string& text)
{
	for (auto i = 0; i < m_ChannelCount * m_Width * m_Height; i++)
	{
		// Number of pixels does not tally
		if (text.length() == 0)
			return 1;

		int number;
		if (parseOneInteger(text, number) != 0)
			return 1;
		m_PixelData.push_back((uint8_t)number);
	}

	return 0;
}


int Image::savePPM(const char* filename)
{
	std::ofstream imageFile;
	imageFile.open(filename, std::ofstream::out);
	if (imageFile.fail())
		return 1;

	imageFile
		<< "P3\n"									// Two-byte magic number
		<< m_Width << " " << m_Height << "\n"		// Image dimensions in pixels
		<< "255\n";									// Maximum value for each color

	// Empty image
	if (m_Width == 0 || m_Height == 0)
		return 0;

	// Write RGB triplets
	int row = 0;
	for (const int& pixel : m_PixelData)
	{
		imageFile << pixel << " ";
		++row;
	}

	return 0;
}

int Image::loadPPM(const char* filename)
{
	std::ifstream imageFile(filename);
	if (imageFile.fail())
		return 1;

	std::string text;

	getline(imageFile, text);	// Two-byte magic number
	if (text != "P3")
		return 1;

	getline(imageFile, text);	// Dimensions
	if (parseDimensions(text) != 0)
		return 1;

	getline(imageFile, text);	// Maximum value for each color
	if (text != "255")
		return 1;

	getline(imageFile, text);	// Pixel Data
	if (parsePixelData(text) != 0)
		return 1;

	// Pre-allocate the expected size of the pixel data
	m_PixelData.reserve( (size_t) m_ChannelCount * m_Width * m_Height );
	return 0;
}

int Image::width() const
{
	return m_Width;
}

int Image::height() const
{
	return m_Height;
}

std::vector<uint8_t>& Image::getImageData()
{
	return m_PixelData;
}
