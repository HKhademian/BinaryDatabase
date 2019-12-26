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
			void *value = nullptr;

			explicit Result(int typ = ERR, std::string msg = std::string(), void *val = nullptr) :
				type(typ), message(std::move(msg)), value(val) {}
		};

		static const auto RESULT_ERR = Result(Result::ERR); // NOLINT(cert-err58-cpp,cppcoreguidelines-interfaces-global-init)
	}
}
#endif
