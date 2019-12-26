#include<cstdarg>
#include<string>
#include <iostream>
#include <fstream>
#include "../MetaInfo/DatabaseInfo.h"
#include "Context.h"
#include "Result.h"

namespace db {
	namespace ctx {
		void Context::open(
			const std::string &name,
			int version,
			OnDatabaseOpen onOpen,
			OnDatabaseCreate onCreate,
			OnDatabaseUpgrade onUpgrade
		) {
			try {
				auto dbInfo = new DatabaseInfo;
				const std::string path = "./db-" + name + "-inf.dat";

				std::ifstream is;
				is.open(path, std::ios::in | std::ios::binary);
				is >> *dbInfo;
				is.close();

				database = dbInfo;
			} catch (...) {
				database = new DatabaseInfo(name);
				saveDatabaseInfo();
				if (onCreate)
					onCreate(*this, version);

			}

			// TODO: some version management
			if (onOpen)
				onOpen(*this, version);
		}

		void Context::clear() {
			delete table;
			table = nullptr;

			delete columns;
			//columns.clear();
			columns = nullptr;

			delete rows;
			//rows.clear();
			rows = nullptr;

			//delete res;
			//res = nullptr;

			args.clear();
		}

		void Context::close() {
			// TODO: some extra saving state process

			clear();

			delete database;
			database = nullptr;
		}

		void Context::saveDatabaseInfo() const {
			const std::string path = "./db-" + database->name + "-inf.dat";
			std::ofstream os;
			os.open(path, std::ios::out | std::ios::trunc | std::ios::binary);
			os << *database;
			os.close();
		}

		void Context::exec(const std::string &cmd, ...) {
			clear();

			//TODO: implement varargs
			va_list vargs;
			va_start(vargs, cmd);
			va_end(vargs);

			snapEval(*this, cmd, Range(0, cmd.size() - 1));
		}

		Context *Context::result(int type, const std::string &message) {
			this->res.type = Result::DONE;
			this->res.message = message;
			return this;
		}

		Context *Context::done(const std::string &message) {
			return result(Result::DONE, message);
		}

		Context *Context::err(const std::string &message) {
			return result(Result::ERR, message);
		}

	}
}