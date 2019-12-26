#include<string>
#include<cstring>
#include "../../MetaInfo/DataType.h"
#include "../../MetaInfo/ColumnInfo.h"
#include "../../MetaInfo/TableInfo.h"
#include "../../utils.h"
#include "Eval.h"
#include "Functions.h"

namespace db {
	namespace ctx {
		std::string parseColTypeName(const std::string &param, size_t sepLoc) {
			Range nameRange = Range(0, sepLoc - 1);
			if (!param2Text(param, nameRange, true)) {
				throw std::invalid_argument("col name required");
			}
			const auto colName = paramSub(param, nameRange);
			if (!checkName(colName)) {
				throw std::invalid_argument("illegal column name");
			}
			return colName;
		}

		DataType parseDataType(const std::string &param, size_t sepLoc) {
			static const auto TYP_BYTE = "byte";
			static const auto TYP_CHAR = "char";
			static const auto TYP_INT = "int";
			static const auto TYP_REAL = "real";
			static const auto TYP_TEXT = "text";

			TypeSize count = 1;
			auto typeRange = Range(sepLoc + 1, param.size() - 1);
			auto countRange = typeRange;
			if (paramFindRange(param, Ranger::lst, countRange)) {
				const auto countParam = paramSub(param, Range(countRange.start + 1, countRange.end - 1));
				count = std::atoi(countParam.c_str()); // NOLINT(cert-err34-c)
				typeRange.end = countRange.start - 1;
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

		ColumnInfo createCol(const std::string &cmd, const Range paramRange) {
			static const auto TYP_SEP = ':';

			const auto &param = paramSub(cmd, paramRange);
			const auto sepLoc = param.find(TYP_SEP);
			if (sepLoc < 0 || sepLoc >= param.size()) {
				throw std::invalid_argument("invalid col and type combination");
			}

			std::string colName = parseColTypeName(param, sepLoc);
			DataType colType = parseDataType(param, sepLoc);

			const ColumnInfo colInfo(colType, colName);
			return colInfo;
		}


		Eval (evalCreateTable) {
			const auto &tableName = parseTableName(cmd, vparams[0]);
			if (context.database->table(tableName) >= 0) {
				throw std::invalid_argument("table already exists");
			}

			TableInfo tableInfo(tableName);
			loopIn(i, 1, vparams.size()) {
				auto range = vparams[i];
				const ColumnInfo columnInfo = createCol(cmd, range);
				if (tableInfo.column(columnInfo.name) >=0) {
					throw std::invalid_argument("duplicate column name");
				}
				tableInfo.columns.push_back(columnInfo);
			}

			context.database->tables.push_back(tableInfo);
			context.saveDatabaseInfo();

			return context.done();
		}

		Eval (evalDeleteTable) {
			if (vparams.size() != 1) {
				throw std::invalid_argument("illegal param count");
			}
			const auto tableName = parseTableName(cmd, vparams[0]);
			const auto tablePos = context.database->table(tableName);
			if (tablePos < 0) {
				throw std::invalid_argument("table does not exists");
			}
			context.database->tables.erase(context.database->tables.begin() + tablePos);
			context.saveDatabaseInfo();
			return Result(Result::DONE);
		}



	}
}