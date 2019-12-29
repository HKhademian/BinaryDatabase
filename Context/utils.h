#ifndef DATABASE_CONTEXT_UTILS_H
#define DATABASE_CONTEXT_UTILS_H

#include <string>
#include <stdexcept>
#include "../MetaInfo/TableInfo.h"
#include "../Data/DataValue.h"
#include "params.h"
#include "Context.h"

namespace db {
	namespace ctx {
		static const auto TYP_SEP = ':';

		bool checkName(const std::string &name);

		std::string parseTableName(const std::string &cmd, Range nameRange);

		std::string parseColName(const std::string &cmd, Range nameRange);

		const TableInfo &getTable(Context &context, const std::string &cmd, Range range);

		const ColumnInfo &getColumn(Context &context, const TableInfo &table, const std::string &cmd, Range range);

		DataValue &parseValue(Context &context, DataValue &val, const std::string &cmd, Range range);

		DataValue parseValue(Context &context, const ColumnInfo &column, const std::string &cmd, Range range);


		std::string parseColTypeName(const std::string &param, size_t sepLoc);

		DataType parseColTypeType(const std::string &param, size_t sepLoc);


		std::string parseTypeParamCol(const std::string &param, Range range);

		DataType parseTypeParamType(const std::string &param, Range range);
	}
}

#endif