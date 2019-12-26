#include<string>
#include<cstring>
#include <iostream>
#include "../../MetaInfo/DataType.h"
#include "../../MetaInfo/ColumnInfo.h"
#include "../../MetaInfo/TableInfo.h"
#include "../../utils.h"
#include "../params.h"
#include "../Result.h"
#include "../Context.h"
#include "Eval.h"
#include "Functions.h"

namespace db {
	namespace ctx {
		Eval (evalSelect) {
			if (vparams.empty() || vparams.size() > 2) {
				throw std::invalid_argument("illegal param count");
			}
			const auto tableName = parseTableName(cmd, vparams[0]);
			const auto tablePos = context.database->table(tableName);
			if (tablePos < 0) {
				throw std::invalid_argument("table does not exists");
			}

			const auto &table = context.database->tables[tablePos];
			context.table = (TableInfo *) &table;

			if (vparams.size() == 1) {
				return context.err(); // TODO: SelectResult
			}

			// TODO: check previous rows
			auto rows = loadRows(table);
			context.rows = new std::vector<DataRow>(rows);

			return eval(context, cmd, vparams[1]);
		}
	}
}