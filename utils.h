#ifndef DATABASE_UTILS_H
#define DATABASE_UTILS_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

namespace db {
	#define self (*this)

#define loopRange(type, i, begin, end) for(type i=(begin); i<(end); i++)
#define loopRangeEQ(type, i, begin, end) for(type i=(begin); i<=(end); i++)

#define loopIn(i, begin, end) loopRange(int, i, begin, end)
#define loop(i, end) loopRange(int, i, 0, end)
#define loopn(i, count) loopRange(int, i, 1, (count)+1)

	size_t trim(const char *str, size_t &start, size_t &end);

	std::vector<std::string> split(const std::string &s, char delimiter);

	bool strcaseequal(const std::string &s1, const std::string &s2);
}
#endif
