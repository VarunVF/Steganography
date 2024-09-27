# Steganography

A simple C++ project that demonstrates how to hide and retrieve secret messages within images using steganography techniques.

## Features

- Least Significant Bit (LSB) Steganography: Hide messages in the least significant bits of image pixels.
- Image File Support: Works with PPM format.
- Message Encoding and Decoding: Encode secret text messages into images and retrieve them with ease.

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

Loading an image:
```cpp
Image image;
image.loadPPM("input.ppm");
```

Saving an image:
```cpp
image.savePPM("output.ppm");
```

Encoding a message:
```cpp
// Example using LSB Insertion
std::string message = "top secret";
image.hideLSB(message);
```

Decoding a message
```cpp
// Example using LSB Insertion
int messageLength = 20;  // Number of chars to look for
std::string result = image.findLSB(messageLength);
```

## Future Features

- Support for more image formats.
- Additional steganography techniques.
