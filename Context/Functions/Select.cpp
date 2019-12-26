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

			std::vector<size_t> &records = context.records;
			records.clear();
			const auto &table = context.database->tables[tablePos];
			context.table = (TableInfo *) &table;

			if (vparams.size() == 1) {
				return RESULT_ERR; // TODO: SelectResult
			}

			// TODO: load record offsets

			const auto query = paramSub(cmd, vparams[1]);
			return eval(context, query);
		}
	}
}