# Steganography

A simple C++ project that demonstrates how to hide and retrieve secret messages within images using steganography techniques.

## Features

- **Least Significant Bit (LSB) Steganography:**
	Hide messages in the least significant bits of image pixels.
- **Image File Support:**
	Works with PPM and PNG formats.
- **Message Encoding and Decoding:**
	Encode secret text messages into images and retrieve them with ease.

## Example

Input PNG file:

<img src="Steganography/test/cpp_logo.png" width=200>


Output:

<img src="Steganography/test/cpp_logo_output.png" width=200>


## Prerequisites

- C++14 or later
- Microsoft Visual Studio 2022


## Installation

Clone the repository:

```bash
git clone https://github.com/VarunVF/Steganography.git steganography-cpp
cd steganography-cpp
```

Compile the project:

In Visual Studio, click on "Build" > "Build Solution".
The executable and intermediates are saved in `$(PlatformDir)bin\`.

## Usage

Hiding a message using PNG format:
```cpp
// Provide number of channels per pixel (4 = RGBA)
Image image(4);
image.loadPNG("input.png");

// Message to be hidden
std::string message = "secret";
image.hideLSB(secret);

// Required for PNG format:
// Update the internal PNG buffer with the new data
image.updatePNG();

// Save the result with the encoded image
image.savePNG("output.png");
```

Recovering a message using PNG format:
```cpp
Image stegfile(4);
stegfile.loadPNG("hidden.png");

// Provide the number of characters to look for (e.g. 6)
std::string secret = stegfile.findLSB(6);
```

Similar functions are available for PPM images.
```cpp
loadPPM(filename);
savePPM(filename);
```

## Future Features

- Support for more image formats.
- Additional steganography techniques.
