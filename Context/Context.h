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

		Context &eval(const Context &context, const std::string &cmd, Range range);

		Context &snapEval(Context &context, const std::string &cmd, Range range);

		typedef void (*OnDatabaseOpen)(Context &context, int version);

		typedef void (*OnDatabaseCreate)(Context &context, int version);

		typedef void (*OnDatabaseMigrate)(Context &context, int version, int oldVersion);

		struct Context {
			~Context();

			explicit Context(const Context *parent = nullptr);

		private:
			const Context *parent = nullptr;
			std::vector<DataValue> arguments;
			std::vector<ColumnInfo> columns;
			std::vector<DataRow> dataRows;
			Result result;
			DatabaseInfo *database = nullptr;
			TableInfo *table = nullptr;

		public:
			const std::vector<DataValue> &args(bool include = true) const;


			const Result &res(bool include = true) const;

			Context &res(int type, const std::string &message);

			Context &done(const std::string &message = "");

			Context &err(const std::string &message = "");

			Context &todo(const std::string &message = "");


			const DatabaseInfo *pdb(bool include = true) const;

			const DatabaseInfo &db(bool include = true) const;


			const std::vector<ColumnInfo> &cols(bool include = true) const;

			Context &cols(std::vector<ColumnInfo> newcols);


			const std::vector<DataRow> &rows(bool include = true) const;

			Context &rows(std::vector<DataRow> newrows);


			const TableInfo *ptbl(bool include = true) const;

			const TableInfo &tbl(bool include = true) const;

			Context &tbl(TableInfo *ptable);


			Context &exec(const std::string &cmd, ...);

			Context &open(
				const std::string &name, int version = 1,
				OnDatabaseOpen onOpen = nullptr,
				OnDatabaseCreate onCreate = nullptr,
				OnDatabaseMigrate onMigrate = nullptr
			);

			Context &close();

			Context &clear();

			void saveDatabaseInfo() const;

			bool hasError();
		};
	}
}
#endif
