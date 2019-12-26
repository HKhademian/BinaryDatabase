#include <string>
#include <stdexcept>
#include "../params.h"
#include "../../MetaInfo/TableInfo.h"
#include "../Context.h"

namespace db {
	namespace ctx {

		bool checkName(const std::string &name) {
			return !name.empty();
		}

		std::string parseTableName(const std::string &cmd, Range nameRange) {
			if (!param2Text(cmd, nameRange, true)) {
				throw std::invalid_argument("table name required");
			}
			auto name = paramSub(cmd, nameRange);
			if (!checkName(name)) {
				throw std::invalid_argument("illegal table name");
			}
			return name;
		}

		std::string parseColName(const std::string &cmd, Range nameRange) {
			if (!param2Text(cmd, nameRange, true)) {
				throw std::invalid_argument("col name required");
			}
			auto name = paramSub(cmd, nameRange);
			if (!checkName(name)) {
				throw std::invalid_argument("illegal col name");
			}
			return name;
		}


		TableInfo &getTable(Context &context, const std::string &cmd, Range range) {
			const auto &tableName = parseTableName(cmd, range);
			const auto tablePos = context.database->table(tableName);
			if (tablePos < 0) {
				throw std::invalid_argument("table does not exists");
			}
			return context.database->tables[tablePos];
		}

		ColumnInfo &getColumn(Context &context, TableInfo &table, const std::string &cmd, Range range) {
			const auto &columnName = parseColName(cmd, range);
			const auto &columnPos = table.column(columnName);
			if (columnPos < 0) {
				throw std::invalid_argument("column does not exists");
			}
			return table.columns[columnPos];
		}

		const ColumnInfo &getColumn(Context &context, const TableInfo &table, const std::string &cmd, Range range) {
			const auto &columnName = parseColName(cmd, range);
			const auto &columnPos = table.column(columnName);
			if (columnPos < 0) {
				throw std::invalid_argument("column does not exists");
			}
			return table.columns[columnPos];
		}

		DataCell parseValue(Context &context, const ColumnInfo &column, const std::string &cmd, Range range) {
			const auto &type = column.type;
			const auto &param = paramSub(cmd, range);
			DataCell cell(column);

			if (isDataType(type, TYPE_BYTE)) {
				//TODO: handle list
				cell.setValueByte(std::stoi(param));
			} else if (isDataType(type, TYPE_INT)) {
				//TODO: handle list
				cell.setValueInt(std::stol(param));
			} else if (isDataType(type, TYPE_REAL)) {
				//TODO: handle list
				cell.setValueReal(std::stod(param));
			} else if (isDataType(type, TYPE_TEXT)) {
				cell.setValueText(param);
			} else {
				throw TypeError();
			}

			return cell;
		}

	}
}