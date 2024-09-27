#include "parseOneInteger.hpp"


int parseOneInteger(std::string& text, int& number)
{
	if (text.length() == 0)
		return 1;

	std::string integerStr;

	// Skip to the first digit
	while (!isdigit(text[0]))
		text.erase(text.begin());

	// Read one integer
	while (isdigit(text[0]))
	{
		integerStr += text[0];
		if (text.length() == 0)
			break;
		text.erase(text.begin());
	}

	number = std::stoi(integerStr);
	return 0;
}
