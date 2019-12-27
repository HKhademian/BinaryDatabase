#ifndef DATABASE_CONTEXT_ARGS_H
#define DATABASE_CONTEXT_ARGS_H

#include<cstdarg>
#include "../Data/DataValue.h"
#include "Context.h"

namespace db {
	namespace ctx {
		static const char ARG_SIGN = '$';

		struct Argument : public DataValue {
			TypeSize index = -1; // index-less = -1
			std::vector<Range> ranges; // available in this ranges

			Argument(TypeSize index, DataType type) : DataValue(type, true), index(index) {}

			Argument() : Argument(TYPE_NONE, -1) {}
		};

		void parseCommandArgs(std::vector<Argument> &args, va_list &vargs, const std::string &cmd);

		/// return arg in that range if available
		const Argument *getArg(const std::vector<Argument> &args, Range range);

		/// getArg with extra paramTrim
		const Argument *getArg(const std::vector<Argument> &args, const std::string &cmd, Range range);

		/// fill value if available
		bool isArg(DataValue &value, const std::vector<Argument> &args, const std::string &cmd, Range range);
	}
}
#endif
