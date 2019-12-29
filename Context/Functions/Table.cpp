#include<string>
#include "../../utils.h"
#include "../utils.h"
#include "Eval.h"

namespace db {
	namespace ctx {
		ColumnInfo createCol(const std::string &cmd, const Range paramRange) {
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
			auto &db = (DatabaseInfo &) context.getDB();
			if (db.tablePos(tableName) >= 0) {
				throw std::invalid_argument("table already exists");
			}

			TableInfo tableInfo(tableName);
			loopIn(i, 1, vparams.size()) {
				auto range = vparams[i];
				const ColumnInfo columnInfo = createCol(cmd, range);
				if (tableInfo.columnPos(columnInfo.name) >= 0) {
					throw std::invalid_argument("duplicate column name");
				}
				tableInfo.columns.push_back(columnInfo);
			}

			db.tables.push_back(tableInfo);

			//TODO: Add Table data/index/... files
			context.saveDatabaseInfo();

			return context.done();
		}

		Eval (evalDeleteTable) {
			if (vparams.size() != 1) {
				throw std::invalid_argument("illegal param count");
			}
			auto &db = (DatabaseInfo &) context.getDB();
			const auto tableName = parseTableName(cmd, vparams[0]);
			const auto tablePos = db.tablePos(tableName);
			if (tablePos < 0) {
				throw std::invalid_argument("tablePos does not exists");
			}
			db.tables.erase(db.tables.begin() + tablePos);

			//TODO: Remove Table data/index/... files
			context.saveDatabaseInfo();

			return context.done();
		}

	}
}