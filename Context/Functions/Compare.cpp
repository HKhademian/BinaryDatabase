#include<string>
#include "../../utils.h"
#include "../../Data/DataTable.h"
#include "../utils.h"
#include "Eval.h"
#include "Functions.h"
#include "../../Data/DataTable.h"

namespace db {
	namespace ctx {
		int cmpEQ(const DataValue &lhs, const DataValue &rhs) { return lhs == rhs;/*lhs.compare(rhs) == 0;*/ }

		int cmpNE(const DataValue &lhs, const DataValue &rhs) { return lhs.compare(rhs) != 0; }

		int cmpGT(const DataValue &lhs, const DataValue &rhs) { return lhs.compare(rhs) > 0; }

		int cmpGE(const DataValue &lhs, const DataValue &rhs) { return lhs.compare(rhs) >= 0; }

		int cmpLT(const DataValue &lhs, const DataValue &rhs) { return lhs.compare(rhs) < 0; }

		int cmpLE(const DataValue &lhs, const DataValue &rhs) { return lhs.compare(rhs) <= 0; }

		Eval (evalCompare) {
			const auto cmp =
				strcaseequal(func, "eq") ? cmpEQ :
				strcaseequal(func, "ne") || strcaseequal(func, "neq") ? cmpNE :
				strcaseequal(func, "gt") || strcaseequal(func, "nle") ? cmpGT :
				strcaseequal(func, "ge") || strcaseequal(func, "nlt") ? cmpGE :
				strcaseequal(func, "lt") || strcaseequal(func, "nge") ? cmpLT :
				strcaseequal(func, "le") || strcaseequal(func, "nlt") ? cmpLE :
				throw std::invalid_argument("invalid compare");
			if (vparams.size() != 2) {
				throw std::invalid_argument("illegal param count");
			}
			const auto &table = *(context.table ?: throw std::invalid_argument("query outside of Select"));
			auto &rows = *(context.rows ?: throw std::invalid_argument("select before compare"));
			const auto &column = getColumn(context, table, cmd, vparams[0]);
			const auto &rhs = parseValue(context, column, cmd, vparams[1]);

			// TODO: check previous rows
			{
				std::vector<ColumnInfo> cols;
				cols.push_back(column);
				loadData(table, cols, rows);
			}

			auto &result = *new std::vector<DataRow>();

			const int rowCount = rows.size();
			loop(i, rowCount) {
				const auto &row = rows[i];
				const auto &lhs = row.atColumn(column);
				if (cmp(lhs, rhs)) {
					result.push_back(rows[i]);
				}
			}

			context.rows = &result;
			return context.done();
		}
	}
}