#include "../utils.h"
#include "utils.h"

namespace db {
	namespace ctx {
		bool checkName(const std::string &name) {
			return !name.empty();
		}

		std::string parseTableName(const std::string &cmd, Range nameRange) {
			if (!param2Text(cmd, nameRange, true)) {
				throw std::invalid_argument("tablePos name required");
			}
			auto name = paramSub(cmd, nameRange);
			if (!checkName(name)) {
				throw std::invalid_argument("illegal tablePos name");
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

		const TableInfo &getTable(Context &context, const std::string &cmd, Range range) {
			const auto &tableName = parseTableName(cmd, range);
			const auto ptable = context.db().table(tableName);
			if (ptable == nullptr) {
				throw std::invalid_argument("table does not exists");
			}
			return *ptable;
		}

		const ColumnInfo &getColumn(Context &context, const TableInfo &table, const std::string &cmd, Range range) {
			const auto &columnName = parseColName(cmd, range);
			const auto &columnPos = table.columnPos(columnName);
			if (columnPos < 0) {
				throw std::invalid_argument("column does not exists");
			}
			return table.columns[columnPos];
		}

		DataValue &parseValue(Context &context, DataValue &value, const std::string &cmd, Range range) {
			const auto &type = value.type();

			if (isDataType(type, TYPE_TEXT)) {
				if (param2Text(cmd, range, false)) { // plain text found
					range.start++, range.end--;
					const auto &param = paramSub(cmd, range, false);
					return value.setValueText(param);
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
				return value.setValue(val, count);
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
				return value.setValue(val, count);
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
				return value.setValue(val, count);
			}

			throw TypeError();
		}

		DataValue parseValue(Context &context, const ColumnInfo &column, const std::string &cmd, Range range) {
			DataValue val(column.type);
			parseValue(context, val, cmd, range);
			return val;
		}

		std::string parseTypeParamCol(const std::string &param, Range range) {
			if (!param2Text(param, range, true)) {
				throw std::invalid_argument("col name required");
			}
			const auto colName = paramSub(param, range);
			if (!checkName(colName)) {
				throw std::invalid_argument("illegal column name");
			}
			return colName;
		}

		DataType parseTypeParamType(const std::string &param, Range range) {
			static const auto TYP_BYTE = "byte";
			static const auto TYP_CHAR = "char";
			static const auto TYP_INT = "int";
			static const auto TYP_REAL = "real";
			static const auto TYP_TEXT = "text";

			Range typeRange(range);
			TypeSize count = 1;
			{
				Range countRange(range);
				if (paramFindRange(param, Ranger::lst, countRange)) {
					const auto countParam = paramSub(param, Range(countRange.start + 1, countRange.end - 1));
					count = std::atoi(countParam.c_str()); // NOLINT(cert-err34-c)
					typeRange.end = countRange.start - 1;
				}
			}
			const auto typeParam = paramSub(param, typeRange);

			return
				strcaseequal(typeParam, TYP_TEXT) ? dataType(TYPE_TEXT, 1) :
				count <= 0 ? throw std::invalid_argument("illegal type count argument") :
				strcaseequal(typeParam, TYP_BYTE) ? dataType(TYPE_BYTE, count) :
				strcaseequal(typeParam, TYP_CHAR) ? dataType(TYPE_BYTE, count) :
				strcaseequal(typeParam, TYP_INT) ? dataType(TYPE_INT, count) :
				strcaseequal(typeParam, TYP_REAL) ? dataType(TYPE_REAL, count) :
				throw TypeError();
		}

		DataType parseColTypeType(const std::string &param, size_t sepLoc) {
			return parseTypeParamType(param, Range(sepLoc + 1, param.size() - 1));
		}

		std::string parseColTypeName(const std::string &param, size_t sepLoc) {
			return parseTypeParamCol(param, Range(0, sepLoc - 1));
		}

	}
}