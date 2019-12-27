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

		Argument *getArg(std::vector<Argument> &args, const std::string &cmd, Range range);

		void parseCommandArgs(std::vector<Argument> &args, va_list &vargs, const std::string &cmd);

	}
}
#endif
