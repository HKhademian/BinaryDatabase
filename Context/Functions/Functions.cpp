#include <string>
#include <stdexcept>
#include "../params.h"
#include "../../MetaInfo/TableInfo.h"
#include "../Context.h"
#include "../../utils.h"

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

		DataCell &parseValue(Context &context, DataCell &cell, const std::string &cmd, Range range) {
			const auto &type = cell.column.type;

			if (isDataType(type, TYPE_TEXT)) {
				if (param2Text(cmd, range, false)) { // plain text found
					range.start++, range.end--;
					const auto &param = paramSub(cmd, range, false);
					cell.setValueText(param);
					return cell;
				}
				throw std::invalid_argument("not valid string argument");
			}

			const auto &count = getTypeCount(type);
			std::vector<Range> vparams;
			if (isParamRange(cmd, Ranger::lst, range)) {
				vparams = paramSplit(cmd, Ranger::lst, range);
			} else {
				vparams.push_back(range);
			}

			if (isDataType(type, TYPE_BYTE)) {
				TypeByte val[count];
				loop(i, count) {
					if (i >= vparams.size()) {
						val[i] = 0;
					} else {
						const auto &param = paramSub(cmd, vparams[i], true);
						val[i] = std::stoi(param);
					}
				}

				cell.setValue(val);
				return cell;
			}

			if (isDataType(type, TYPE_INT)) {
				TypeInt val[count];
				loop(i, count) {
					if (i >= vparams.size()) {
						val[i] = 0;
					} else {
						const auto &param = paramSub(cmd, vparams[i], true);
						val[i] = std::stoi(param);
					}
				}

				cell.setValue(val);
				return cell;
			}

			if (isDataType(type, TYPE_REAL)) {
				TypeReal val[count];
				loop(i, count) {
					if (i >= vparams.size()) {
						val[i] = 0;
					} else {
						const auto &param = paramSub(cmd, vparams[i], true);
						val[i] = std::stof(param); //TODO: maybe double
					}
				}

				cell.setValue(val);
				return cell;
			}

			throw TypeError();
		}

		DataCell parseValue(Context &context, const ColumnInfo &column, const std::string &cmd, Range range) {
			DataCell cell(column);
			parseValue(context, cell, cmd, range);
			return cell;
		}

	}
}