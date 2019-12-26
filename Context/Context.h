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

		Result eval(const Context &ctx, const std::string &cmd);

		Result snapEval(Context &ctx, const std::string &cmd);

		typedef void (*OnDatabaseOpen)(Context &context, int version);

		typedef void (*OnDatabaseCreate)(Context &context, int version);

		typedef void (*OnDatabaseUpgrade)(Context &context, int version, int oldVersion);

		class Context {
		public:
			DatabaseInfo *database = nullptr;
			TableInfo *table = nullptr;
			std::vector<ColumnInfo> columns;
			std::vector<size_t> records;
			std::vector<void *> args;

			Result exec(const std::string &cmd, ...);
//			void insert(const DataRow &row);

			void open(
				const std::string &name, int version = 1,
				OnDatabaseOpen onOpen = nullptr,
				OnDatabaseCreate onCreate = nullptr,
				OnDatabaseUpgrade onUpgrade = nullptr
			);

			void close();

			void saveDatabaseInfo() const;

			void insert(const DataRow &row);
		};
	}
}
#endif
