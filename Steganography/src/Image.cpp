#include <iostream>
#include <fstream>
#include <string>

#include "Image.hpp"
#include "parseOneInteger.hpp"


Image::Image()
	: m_Width(0), m_Height(0), m_PixelData()
{
}

int Image::hideLSB(std::string& secret)
{
	// Check if the image is large enough to hide this message
	size_t requiredSize = secret.length() * 8;
	size_t requiredPixels = (size_t) std::ceil( (float)requiredSize / 3 );

	size_t actualSize = m_PixelData.size();
	size_t actualPixels = (size_t) std::ceil( (float)actualSize / 3 );

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

std::string Image::findLSB(int secretLength)
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

int Image::parseDimensions(std::string& text)
{
	if (parseOneInteger(text, m_Width) != 0
	 || parseOneInteger(text, m_Height) != 0)
		return 1;
	return 0;
}

int Image::parsePixelData(std::string& text)
{
	for (auto i = 0; i < 3 * m_Width * m_Height; i++)
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
	m_PixelData.reserve( (size_t) 3 * m_Width * m_Height );
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
