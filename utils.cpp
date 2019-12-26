#include <iostream>
#include "MetaInfo/DataType.h"
#include "utils.h"

namespace db {
	size_t trim(const char *str, size_t &start, size_t &end) {
		while (start <= end && str[start] == ' ') start++;
		while (start <= end && str[end] == ' ') end++;
		return start > end ? 0 : 1 + end - start;
	}

	std::vector<std::string> split(const std::string &s, char delimiter) {
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(s);
		while (std::getline(tokenStream, token, delimiter)) {
			tokens.push_back(token);
		}
		return tokens;
	}

	bool strcaseequal(const std::string &str1, const std::string &str2) {
		const auto &cmp = [](const char &c1, const char &c2) {
			return c1 == c2 || std::toupper(c1) == std::toupper(c2);
		};

		return (
				(str1.size() == str2.size()) &&
				std::equal(str1.begin(), str1.end(), str2.begin(), cmp)
		);
	}
}
