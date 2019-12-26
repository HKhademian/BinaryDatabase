#include<string>
#include "../../utils.h"
#include "Eval.h"
#include "Functions.h"

namespace db {
	namespace ctx {
		int compare(const DataCell &lhs, const DataCell &rhs) {
			if (lhs.column.type != rhs.column.type) {
				throw TypeError("type(rhs!=lhs)");
			}
			const auto &type = lhs.column.type;
			if (isDataType(type, TYPE_BYTE)) {
				const auto &l = *(TypeByte *) lhs.getValue(), &r = *(TypeByte *) rhs.getValue();
				return l - r;
			}
			if (isDataType(type, TYPE_INT)) {
				const auto &l = *(TypeInt *) lhs.getValue(), &r = *(TypeInt *) rhs.getValue();
				return l - r;
			}
			if (isDataType(type, TYPE_REAL)) {
				const auto &l = *(TypeReal *) lhs.getValue(), &r = *(TypeReal *) rhs.getValue();
				return l > r ? 1 : l < r ? -1 : 0;
			}
			if (isDataType(type, TYPE_TEXT)) {
				const auto &l = *(TypeText *) lhs.getValue(), &r = *(TypeText *) rhs.getValue();
				return l.compare(r);
			}
			throw TypeError();
		};

		int cmpEQ(const DataCell &lhs, const DataCell &rhs) { return compare(lhs, rhs) == 0; }

		int cmpNE(const DataCell &lhs, const DataCell &rhs) { return compare(lhs, rhs) != 0; }

		int cmpGT(const DataCell &lhs, const DataCell &rhs) { return compare(lhs, rhs) > 0; }

		int cmpGE(const DataCell &lhs, const DataCell &rhs) { return compare(lhs, rhs) >= 0; }

		int cmpLT(const DataCell &lhs, const DataCell &rhs) { return compare(lhs, rhs) < 0; }

		int cmpLE(const DataCell &lhs, const DataCell &rhs) { return compare(lhs, rhs) <= 0; }

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
			const auto &rows = *(context.rows ?: throw std::invalid_argument("select before compare"));
			const auto &column = getColumn(context, table, cmd, vparams[0]);
			const auto &rhs = parseValue(context, column, cmd, vparams[1]);

			std::vector<DataRow> result;
			const auto rowCount = rows.size();
			if (rowCount > 0) {
				for (size_t i = rowCount - 1; i >= 0; i--) {
					const auto &row = rows[i];
					const auto &lhs = row.atColumn(column);
					if (cmp(lhs, rhs)) {
						result.push_back(rows[i]);
					}
				}
			}

			//TODO: SelectResult
			return context.done();
		}
	}
}