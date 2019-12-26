#include<cstdarg>
#include<string>
#include<cstring>
#include <cstring>
#include <iostream>
#include <fstream>
#include "../MetaInfo/DataType.h"
#include "../MetaInfo/ColumnInfo.h"
#include "../MetaInfo/TableInfo.h"
#include "../MetaInfo/DatabaseInfo.h"
#include "../Data/DataRow.h"
#include "../utils.h"
#include "Context.h"
#include "utils.h"
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

		void Context::close() {
			// TODO: some extra saving state process

			delete database;
			database = nullptr;

			delete table;
			table = nullptr;

			columns.clear();

			records.clear();
		}

		void Context::saveDatabaseInfo() const {
			const std::string path = "./db-" + database->name + "-inf.dat";
			std::ofstream os;
			os.open(path, std::ios::out | std::ios::trunc | std::ios::binary);
			os << *database;
			os.close();
		}

		Result Context::exec(const std::string &cmd, ...) {
			std::vector<void *> args;

			//TODO: implement
			va_list vargs;
			va_start(vargs, cmd);
			va_end(vargs);

			//TODO: implement
			return snapEval(*this, cmd);
		}

		void Context::insert(const DataRow &row) {
			//TODO: implement
			throw std::logic_error("not implemented yet!");
		}

	}
}