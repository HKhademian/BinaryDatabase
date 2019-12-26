#ifndef DATABASE_CONTEXT_H
#define DATABASE_CONTEXT_H

#include<string>
#include "../MetaInfo/DatabaseInfo.h"
#include "../MetaInfo/TableInfo.h"
#include "../MetaInfo/ColumnInfo.h"
#include "../Data/DataRow.h"
#include "params.h"
#include "Result.h"

namespace db {
	namespace ctx {
		class Context;

		Context *eval(const Context &context, const std::string &cmd, Range range);

		Context *snapEval(Context &context, const std::string &cmd, Range range);

		typedef void (*OnDatabaseOpen)(Context &context, int version);

		typedef void (*OnDatabaseCreate)(Context &context, int version);

		typedef void (*OnDatabaseUpgrade)(Context &context, int version, int oldVersion);

		class Context {
		public:
			Result res;
			std::vector<void *> args;

			DatabaseInfo *database = nullptr;
			TableInfo *table = nullptr;
			std::vector<DataRow> *rows = nullptr;
			std::vector<ColumnInfo> *columns = nullptr;

			Context *exec(const std::string &cmd, ...);

			void open(
				const std::string &name, int version = 1,
				OnDatabaseOpen onOpen = nullptr,
				OnDatabaseCreate onCreate = nullptr,
				OnDatabaseUpgrade onUpgrade = nullptr
			);

			void clear();

			void close();

			void saveDatabaseInfo() const;

			Context *result(int type, const std::string &message);

			Context *done(const std::string &message = "");

			Context *err(const std::string &message = "");

			bool hasError();
		};
	}
}
#endif
