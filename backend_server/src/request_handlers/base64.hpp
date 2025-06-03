#ifndef BASE_64_FUNCTIONAL_HPP
#define BASE_64_FUNCTIONAL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <bitset>


namespace Base64
{
	const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	std::string encode(std::string text)
	{
		std::string text_binary = "";
		for (unsigned int i = 0; i < text.size(); i ++)
			text_binary += std::bitset<8>(char(text[i])).to_string();

		std::string text_encoded = "";
		unsigned int i;
		for (i = 0; i < (text_binary.size()/6); i++)
			text_encoded += chars[std::bitset<6>(text_binary.substr(i*6, 6)).to_ulong()];

		int remainder = text.size() % 3;
		if (remainder == 0)
			return text_encoded;

		text_binary.resize((i + 1)*6, '0');
		text_encoded += chars[std::bitset<6>(text_binary.substr(i*6, 6)).to_ulong()];
		if (remainder == 1)
			text_encoded += "==";
		if (remainder == 2)
			text_encoded += "=";
		return text_encoded;
	}
}

#endif //BASE_64_FUNCTIONAL_HPP