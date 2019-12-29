#include<string>
#include<cstring>
#include "../../Data/DataTable.h"
#include "../../utils.h"
#include "../utils.h"
#include "Eval.h"

namespace db {
	namespace ctx {
		DataCell &setDataCell(Context &context, DataRow &row, const std::string &cmd, Range range) {
			const auto parts = paramSplit(cmd, Ranger::non, range, '=');
			if (parts.size() != 2) {
				throw std::invalid_argument("illegal data set param => (%COL% = %VAL%)");
			}
			const auto &col = getColumn(context, row.table, cmd, parts[0]);
			auto &cell = *row.atColumn(col);

			parseValue(context, (DataValue &) cell, cmd, parts[1]);

			return cell;
		}

		Eval (evalInsert) {
			if (vparams.size() < 2) {
				throw std::invalid_argument("provide at lease on data set");
			}
			const auto &table = getTable(context, cmd, vparams[0]);

			loopIn(i, 1, vparams.size()) {
				auto setRange = vparams[i];
				if (!isParamRange(cmd, Ranger::set, setRange)) {
					throw std::invalid_argument("illegal param data set => { ... }");
				}
				const auto fields = paramSplit(cmd, Ranger::set, setRange);
				DataRow row(table);
				row.setFree(false);
				std::vector<ColumnInfo> columns;
				for (const auto &field : fields) {
					auto &cell = setDataCell(context, row, cmd, field);
					columns.push_back(cell.column);
				}

				auto stream = table.openDataOutputStream();
				row.writeData(stream, columns);
				stream.close();
			}

			return context.done();
		}

		Eval (evalUpdate) {
			if (vparams.size() != 3) {
				throw std::invalid_argument("provide exacly 3 params");
			}
			const auto &table = getTable(context, cmd, vparams[0]);

			auto setRange = vparams[1];
			if (!isParamRange(cmd, Ranger::set, setRange)) {
				throw std::invalid_argument("illegal param data set => { ... }");
			}
			const auto fields = paramSplit(cmd, Ranger::set, setRange);
			if (fields.empty()) {
				throw std::invalid_argument("set something");
			}

			context.setTable(&table);
			context.setRows(loadRows(table));

			auto &query = eval(context, cmd, vparams[2]);
			if (query.hasError()) {
				return context.err("error in query");
			}

			auto &rows = query.getRows(false);
			std::vector<ColumnInfo> columns;
			for (auto row :rows) {
				for (const auto &field : fields) {
					auto &cell = setDataCell(context, row, cmd, field);
					columns.push_back(cell.column);
				}
			}
			updateDataRows(table, columns, rows);

			return context.done();
		}

		Eval (evalRemove) {
			if (vparams.size() != 2) {
				throw std::invalid_argument("illegal param count (exactly 2)");
			}
			const auto &table = getTable(context, cmd, vparams[0]);

			auto &query = eval(context, cmd, vparams[1]);
			if (query.hasError()) {
				return context.err("error in query");
			}

			removeDataRows(table, (std::vector<RowInfo> &) query.getRows());

			return context.done();
		}

	}
}