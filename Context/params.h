#ifndef DATABASE_CONTEXT_PARAMS_H
#define DATABASE_CONTEXT_PARAMS_H

#include <vector>
#include <string>

namespace db {
	namespace ctx {
		extern const char paramSplitter;

//		typedef std::pair<size_t, size_t> Range;
		class Range {
		public:
			size_t start = 0, end = 0;

			explicit Range(const size_t s, const size_t e) : start(s), end(e) {}

			Range(const Range &range) = default;

			Range() = default;

			Range operator+(const Range &rhs) const;
		};

		class Ranger {
		public:
			static const Ranger non;
			static const Ranger str1, str2, str3;
			static const Ranger lst, set, func;

			const bool dual, none;
			const char start, end;

			explicit Ranger(const char s, const char e)
					: start(s), end(e), dual(s == e), none(s == 0 || e == 0) {}

			explicit Ranger(const char c)
					: Ranger(c, c) {}
		};


		size_t paramTrim(const std::string &str, Range &range);

		std::vector<Range>
		paramSplit(const std::string &str, const Ranger &ranger, Range &range, char splitter = paramSplitter);

		bool paramFindRange(const std::string &str, const Ranger &ranger, Range &range);

		bool isParamRange(const std::string &str, const Ranger &ranger, Range &range);

		bool param2Text(const std::string &str, Range &range, bool bare = false);

		std::string paramSub(const std::string &str, const Range &range, bool trim = true);
	}
}

#endif
