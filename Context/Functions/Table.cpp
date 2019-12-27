#include<string>
#include "../../utils.h"
#include "../utils.h"
#include "Eval.h"

namespace db {
	namespace ctx {
		ColumnInfo createCol(const std::string &cmd, const Range paramRange) {
			static const auto TYP_SEP = ':';
			const auto &param = paramSub(cmd, paramRange);
			const auto sepLoc = param.find(TYP_SEP);
			if (sepLoc < 0 || sepLoc >= param.size()) {
				throw std::invalid_argument("invalid col and type combination");
			}
			std::string colName = parseColTypeName(param, sepLoc);
			DataType colType = parseColTypeType(param, sepLoc);
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
				if (tableInfo.column(columnInfo.name) >= 0) {
					throw std::invalid_argument("duplicate column name");
				}
				tableInfo.columns.push_back(columnInfo);
			}

			context.database->tables.push_back(tableInfo);

			//TODO: Add Table data/index/... files
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
			
			//TODO: Remove Table Files
			context.saveDatabaseInfo();

			return context.done();
		}

	}
}