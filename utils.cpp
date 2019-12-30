#include "utils.h"

namespace db {
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
