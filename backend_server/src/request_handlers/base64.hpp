#ifndef BASE_64_FUNCTIONAL_HPP
#define BASE_64_FUNCTIONAL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <bitset>


namespace Base64
{
	const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int getCharIndex(char required)
	{
		for (unsigned int i = 0; i < 64; i++)
			if (chars[i] == required)
				return i;
		return -1;
	}

	std::string encode(std::string text)
	{
		int remainder = text.size() % 3;

		std::string text_binary = "";
		for (unsigned int i = 0; i < text.size(); i ++)
			text_binary += std::bitset<8>(char(text[i])).to_string();
		text_binary.resize(text_binary.size() + ((3 - remainder)%3)*2, '0');

		std::string text_encoded = "";
		for (unsigned int i = 0; i < (text_binary.size()/6); i++)
			text_encoded += chars[std::bitset<6>(text_binary.substr(i*6, 6)).to_ulong()];

		if (remainder == 0)
			return text_encoded;

		if (remainder == 1)
			text_encoded += "==";
		if (remainder == 2)
			text_encoded += "=";
		return text_encoded;
	}

	std::string decode(std::string text_encoded)
	{
		int equels_count = 0;
		if (text_encoded.find("==") != std::string::npos)
			equels_count = 2;
		else if (text_encoded.find("=") != std::string::npos)
			equels_count = 1;

		std::string text_binary = "";
		for (unsigned int i = 0; i < (text_encoded.size() - equels_count); i ++)
			text_binary += std::bitset<6>(getCharIndex(text_encoded[i])).to_string();
		text_binary.resize(text_binary.size() - equels_count*2);

		std::string text_decoded = "";
		
		for (unsigned int i = 0; i < (text_binary.size()/8); i++)
			text_decoded += char(std::bitset<8>(text_binary.substr(i*8, 8)).to_ulong());

		return text_decoded;
	}
}

#endif //BASE_64_FUNCTIONAL_HPP