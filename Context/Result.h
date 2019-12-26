#ifndef DATABASE_CONTEXT_RESULT_H
#define DATABASE_CONTEXT_RESULT_H

#include <string>

namespace db {
	namespace ctx {

		class Result {
		public:
			static const int ERR = 0;
			static const int DONE = 1;

			int type = ERR;
			std::string message;

			explicit Result(int typ = ERR, std::string msg = std::string()) :
				type(typ), message(std::move(msg)) {}
		};
	}
}
#endif
