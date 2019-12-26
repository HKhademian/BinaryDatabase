#include <stdexcept>
#include "../utils.h"
#include "params.h"

namespace db {
	namespace ctx {
		const char paramSplitter = ',';
		// const char skipChar = '\\';
		const Ranger Ranger::non('\0'); // NOLINT(cert-err58-cpp)
		const Ranger Ranger::str1('"', '"'); // NOLINT(cert-err58-cpp)
		const Ranger Ranger::str2('\''); // NOLINT(cert-err58-cpp)
		const Ranger Ranger::str3('`'); // NOLINT(cert-err58-cpp)
		const Ranger Ranger::lst('[', ']'); // NOLINT(cert-err58-cpp)
		const Ranger Ranger::set('{', '}'); // NOLINT(cert-err58-cpp)
		const Ranger Ranger::func('(', ')'); // NOLINT(cert-err58-cpp)


		size_t paramTrim(const std::string &str, Range &range) {
			while (range.start <= range.end && str[range.start] == ' ') range.start++;
			while (range.start <= range.end && str[range.end] == ' ') range.end--;
			return range.end < range.start ? 0 : 1 + range.end - range.start;
		}


		std::vector<Range> paramSplit(const std::string &str, const Ranger &ranger, Range &range, char splitter) {
			if (!ranger.none) {
				while (range.start <= range.end && str[range.start] != ranger.start)range.start++;
				while (range.start <= range.end && str[range.end] != ranger.end)range.end--;
				if (str[range.start] != ranger.start || str[range.end] != ranger.end) {
					throw std::invalid_argument("bad range");
				}
				range.start++, range.end--;
			}

			std::vector<Range> ranges;
			if (range.start > range.end) {
				return ranges;
			}

			size_t last = range.start;
			loopRangeEQ(size_t, i, last, range.end) {
				const char ch = str[i];

				if (ch == splitter) {
					Range chunk(last, i - 1);
					last = i + 1;
					paramTrim(str, chunk);
					ranges.push_back(chunk); // NOLINT(hicpp-use-emplace,modernize-use-emplace)
				}

				for (const auto chk: {Ranger::str1, Ranger::str2, Ranger::str3, Ranger::lst, Ranger::set, Ranger::func}) {
					if (ch == chk.start) {
						Range chunk(i, range.end);
						if (paramFindRange(str, chk, chunk)) {
							i = chunk.end;
						}
						break;
					}
				}
			}

			Range chunk = Range(last, range.end);
			paramTrim(str, chunk);
			ranges.push_back(chunk); // NOLINT(hicpp-use-emplace,modernize-use-emplace)

			return ranges;
		}

		bool paramFindRange(const std::string &str, const Ranger &ranger, Range &range) {
			while (range.start <= range.end && str[range.start] != ranger.start)range.start++;
			if (str[range.start] != ranger.start) {
				return false;
			}

			const bool inStr =
				(ranger.start == Ranger::str1.start) ||
				(ranger.start == Ranger::str2.start) ||
				(ranger.start == Ranger::str3.start);

			loopRangeEQ(size_t, i, range.start + 1, range.end) {
				const char ch = str[i];
				if (ch == ranger.end) {
					range.end = i;
					return true;
				}
				if (inStr) {
					continue;
				}
				for (const auto chk: {Ranger::str1, Ranger::str2, Ranger::str3, Ranger::lst, Ranger::set, Ranger::func}) {
					if (ch == chk.start) {
						Range chunk(i, range.end);
						if (!paramFindRange(str, chk, chunk)) {
							throw std::invalid_argument("bad range start");
							return false;
						}
						i = chunk.end;
						break;
					} else if (ch == chk.end) {
						throw std::invalid_argument("bad range close");
						return false;
						break;
					}
				}
			}

			//throw std::invalid_argument("bad range");
			return false;
		}

		bool isParamRange(const std::string &str, const Ranger &ranger, Range &range) {
			Range src(range);
			paramTrim(str, src);
			Range req(src);
			if (!paramFindRange(str, ranger, req) ||
			    src.start != req.start || src.end != req.end) {
				return false; // not found
			}
			range = req; // to new pos
			return true;
		}

		bool param2Text(const std::string &str, Range &range, bool bare) {
			paramTrim(str, range);
			const Range req(range.start, range.end);
			try {
				for (auto &ranger : {Ranger::str1, Ranger::str2, Ranger::str3}) {
					if (isParamRange(str, ranger, range = req)) {
						if (bare) { range.start++, range.end--; } // remove ranger
						return true;
					}
				}
			} catch (...) {}
			// bare text ( if allowed )
			range = req;
			return bare;
		}

		std::string paramSub(const std::string &str, const Range &range, bool trim) {
			Range rng = range;
			if (trim) { paramTrim(str, rng); }
			return str.substr(rng.start, rng.end - rng.start + 1);
		}

		Range Range::operator+(const Range &rhs) const {
			return Range(
				this->start + rhs.start,
				this->start + rhs.end
			);
		}
	}
}