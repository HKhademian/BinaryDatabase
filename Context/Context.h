#ifndef DATABASE_CONTEXT_H
#define DATABASE_CONTEXT_H

#include<string>
#include "../MetaInfo/MetaInfo.h"
#include "../Data/Data.h"
#include "params.h"
#include "Result.h"
#include "Argument.h"

namespace db {
	namespace ctx {
		class Context;

		Context &eval(Context &context, const std::string &cmd, Range range);

		Context &snapEval(Context &context, const std::string &cmd, Range range);

		typedef void (*OnDatabaseOpen)(Context &context, int version);

		typedef void (*OnDatabaseCreate)(Context &context, int version);

		typedef void (*OnDatabaseMigrate)(Context &context, int version, int oldVersion);

		struct Context {
			~Context();

			explicit Context(Context *parent = nullptr);

		private:
			Context *parent = nullptr;
			std::vector<Argument> arguments;
			std::vector<ColumnInfo> columns;
			std::vector<DataRow> dataRows;
			Result result;
			DatabaseInfo *database = nullptr;
			TableInfo *table = nullptr;

		public:
			const std::vector<Argument> &args(bool include = true) const;


			const Result &res(bool include = true) const;

			Context &res(int type, const std::string &message);

			Context &done(const std::string &message = "");

			Context &err(const std::string &message = "");

			Context &todo(const std::string &message = "");


			const DatabaseInfo *getpDB(bool include = true) const;

			const DatabaseInfo &getDB(bool include = true) const;


			std::vector<ColumnInfo> &getCols(bool include = true);

			Context &setCols(std::vector<ColumnInfo> newcols);


			std::vector<DataRow> &getRows(bool include = true);

			Context &setRows(std::vector<DataRow> newrows);


			TableInfo *getpTable(bool include = true);

			TableInfo &getTable(bool include = true);

			Context &setTable(const TableInfo *ptable);


			Context &exec(const char *cmd, ...);

			Context &exec(const std::string &cmd, ...);

			/**
			 * if there is varargs in another function pass it.
			 * note: you must close it your self
			 */
			Context &execArgs(const std::string &cmd, va_list &vargs);

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
