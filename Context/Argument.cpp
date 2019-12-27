#include "../utils.h"
#include "utils.h"
#include "Argument.h"

namespace db {
	namespace ctx {
		bool isArgRange(const std::string &cmd, Range &range) {
			Range argRange(range);
			paramTrim(cmd, argRange);
			if (paramFindRange(cmd, Ranger::set, argRange)) {
				if (argRange.start > 0 && cmd[argRange.start - 1] == ARG_SIGN) {
					range = argRange;
					return true;
				}
			}
			return false;
		}

		void parseArgRanges(std::vector<Range> &args, va_list &vargs, const std::string &cmd, const Range &cmdRange) {
			Range range(cmdRange);
			paramTrim(cmd, range);

			while (range.start <= cmdRange.end) {
				// std::cerr << "Checking in range [" << range.start << "," << range.end << "]" << std::endl;
				if (paramFindRange(cmd, Ranger::set, range)) {
					if (isArgRange(cmd, range)) {
						// std::cerr << "New Arg Found [" << range.start << "," << range.end << "]=" << paramSub(cmd, range) << std::endl;
						args.push_back(range);
					} else {
						range.start++, range.end--;
						parseArgRanges(args, vargs, cmd, range);
					}
					range.start = range.end + 1;
					range.end = cmdRange.end;
				} else {
					range.start = cmdRange.end + 1;
				}
			}
		}

		Argument parseArgDataRange(va_list &vargs, const std::string &cmd, Range range) {
			const auto &vparams = paramSplit(cmd, Ranger::set, range, TYP_SEP);
			if (vparams.size() > 2) throw std::invalid_argument("invalid index and type combination");

			DataType type = parseTypeParamType(cmd, vparams[vparams.size() - 1]);
			TypeSize index = vparams.size() == 1 ? -1 : std::stoi(paramSub(cmd, vparams[0], true)) - 1;
			return Argument(index, type);
		}

		void parseCommandArgs(std::vector<Argument> &args, va_list &vargs, const std::string &cmd) {
			args.clear();

			std::vector<Range> ranges;
			parseArgRanges(ranges, vargs, cmd, Range(cmd));

			// extract all arguments parameter and assign them their index
			int index = 0, max = 0;
			for (auto &range: ranges) {
				auto argData = parseArgDataRange(vargs, cmd, range);
				if (argData.index == -1) {
					argData.index = index++;
				}
				if (max < argData.index) {
					loopIn(i, max + 1, argData.index) {
						args.push_back(Argument(i, TYPE_NONE)); // NOLINT(hicpp-use-emplace,modernize-use-emplace)
					}
					max = argData.index;
				}

				bool found = false;
				for (auto &lastArgs : args) {
					if (lastArgs.index == argData.index) { // had it before
						if (lastArgs.type() == TYPE_NONE && argData.type() != TYPE_NONE) {
							lastArgs.type(argData.type());
						}
						if (lastArgs.type() != argData.type()) { throw std::invalid_argument("arguments type is not match"); }
						lastArgs.ranges.push_back(range);
						found = true;
						break;
					}
				}
				if (!found) {  // new argument
					argData.ranges.clear();
					argData.ranges.push_back(range);
					args.push_back(argData);
				}
			}

			for (auto &arg: args) {
				const auto &type = arg.type();

				if (isDataType(type, TYPE_TEXT)) {
					char *val = va_arg(vargs, char*);
					arg.setValueText(std::string(val));
					continue;
				}

				const auto &count = getTypeCount(type);
				if (isDataType(type, TYPE_BYTE)) {
					if (count == 1)
						try { // no ypu can pass pointers even for one
							auto val = va_arg(vargs, int); //REMINDER: cannot receive char (not char*)
							arg.setValueByte(val);
							continue;
						} catch (...) {}
					auto *val = va_arg(vargs, TypeByte *);
					arg.setValueByte(val, count);
					continue;
				}
				if (isDataType(type, TYPE_INT)) {
					if (count == 1)
						try { // no ypu can pass pointers even for one
							auto val = va_arg(vargs, TypeInt);
							arg.setValueInt(val);
							continue;
						} catch (...) {}
					auto *val = va_arg(vargs, TypeInt *);
					arg.setValueInt(val, count);
					continue;
				}
				if (isDataType(type, TYPE_REAL)) {
					if (count == 1)
						try { // no ypu can pass pointers even for one
							auto val = va_arg(vargs, TypeReal);// REMINDER: cannot receive float (only double)
							arg.setValueReal(val);
							continue;
						} catch (...) {}
					auto *val = va_arg(vargs, TypeReal *); // REMINDER: no problem in pointer passing
					arg.setValueReal(val, count);
					continue;
				}
				throw TypeError("hole in indexed arguments");
			}

		}

		const Argument *getArg(const std::vector<Argument> &args, Range range) {
			for (auto &arg : args) {
				for (const auto &argRange: arg.ranges) {
					if (argRange.start == range.start && argRange.end == range.end) {
						return &arg;
					}
				}
			}
			return nullptr;
		}

		const Argument *getArg(const std::vector<Argument> &args, const std::string &cmd, Range range) {
			paramTrim(cmd, range);
			for (auto &arg : args) {
				for (const auto &argRange: arg.ranges) {
					if (argRange.start == range.start && argRange.end == range.end) {
						return &arg;
					}
				}
			}
			return getArg(args, range);
		}

		bool isArg(DataValue &value, const std::vector<Argument> &args, const std::string &cmd, Range range) {
			auto *arg = getArg(args, cmd, range);
			if (arg == nullptr) return false;
			value.setValue(*arg);
			return true;
		}
	}
}