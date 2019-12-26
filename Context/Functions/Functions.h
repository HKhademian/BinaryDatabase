#ifndef DATABASE_CONTEXT_FUNCTIONS_H
#define DATABASE_CONTEXT_FUNCTIONS_H

#include <string>
#include <stdexcept>
#include "../params.h"

namespace db {
	namespace ctx {
		bool checkName(const std::string &name);

		std::string parseTableName(const std::string &cmd, Range nameRange);

		std::string parseColName(const std::string &cmd, Range nameRange);

		TableInfo &getTable(Context &context, const std::string &cmd, Range range);

		ColumnInfo &getColumn(Context &context, TableInfo &table, const std::string &cmd, Range range);

		const ColumnInfo &getColumn(Context &context, const TableInfo &table, const std::string &cmd, Range range);

		DataCell &parseValue(Context &context, DataCell &cell, const std::string &cmd, Range range);

		DataCell parseValue(Context &context, const ColumnInfo &column, const std::string &cmd, Range range);
	}
}
#endif
