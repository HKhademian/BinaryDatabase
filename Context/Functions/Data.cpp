#include<string>
#include<cstring>
#include "../../Data/DataTable.h"
#include "../../utils.h"
#include "../utils.h"
#include "Eval.h"

namespace db {
	namespace ctx {
		void setDataCell(Context &context, DataRow &row, const std::string &cmd, Range range) {
			const auto parts = paramSplit(cmd, Ranger::non, range, '=');
			if (parts.size() != 2) {
				throw std::invalid_argument("illegal data set param => (%COL% = %VAL%)");
			}
			const auto &col = getColumn(context, row.table, cmd, parts[0]);
			auto &cell = row.atColumn(col);

			parseValue(context, cell, cmd, parts[1]);
		}

		Eval (evalInsert) {
			if (vparams.size() < 2) {
				throw std::invalid_argument("provide at lease on data set");
			}
			const auto &table = getTable(context, cmd, vparams[0]);

			size_t done = 0;
			loopIn(i, 1, vparams.size()) {
				auto setRange = vparams[i];
				if (!isParamRange(cmd, Ranger::set, setRange)) {
					throw std::invalid_argument("illegal param data set => { ... }");
				}
				const auto fields = paramSplit(cmd, Ranger::set, setRange);
				DataRow row(table);
				row.setFree(false);
				for (const auto &field : fields) {
					setDataCell(context, row, cmd, field);
				}
				insertData(row);
				done++;
			}

			return context.done();
		}

		Eval (evalRemove) {
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
			return context.done();
		}

	}
}