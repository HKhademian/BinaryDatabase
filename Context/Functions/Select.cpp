#include<string>
#include "../../Data/DataTable.h"
#include "../utils.h"
#include "Eval.h"

namespace db {
	namespace ctx {
		std::vector<DataRow> &join(std::vector<DataRow> &result, const std::vector<DataRow> &source, const std::vector<DataRow> &target);

		std::vector<DataRow> &merge(std::vector<DataRow> &result, const std::vector<DataRow> &source, const std::vector<DataRow> &target);

		std::vector<DataRow> &delta(std::vector<DataRow> &result, const std::vector<DataRow> &source, const std::vector<DataRow> &target);


		std::vector<DataRow> &join(std::vector<DataRow> &result, const std::vector<DataRow> &source, const std::vector<DataRow> &target) {
			if (&result != &source) {
				result = std::vector<DataRow>(source);
				//result.insert(result.end(), source.begin(), source.end());
			}
			return delta(result, source, target);
		}

		std::vector<DataRow> &merge(std::vector<DataRow> &result, const std::vector<DataRow> &source, const std::vector<DataRow> &target) {
			for (const auto &elt:target) {
				bool exists = false;
				for (const auto &els : source) {
					if (elt.unique() == els.unique()) {
						exists = true;
						break;
					}
				}
				if (exists) {
					result.push_back(elt);
				}
			}
			return result;
		}

		std::vector<DataRow> &delta(std::vector<DataRow> &result, const std::vector<DataRow> &source, const std::vector<DataRow> &target) {
			for (const auto &els : source) {
				bool exists = false;
				for (const auto &elt:target) {
					if (elt.unique() == els.unique()) {
						exists = true;
						break;
					}
				}
				if (!exists) {
					result.push_back(els);
				}
			}
			return result;
		}


		/// Select(tableName, [columns], query)
		Eval (evalSelect) {
			if (vparams.empty() || vparams.size() > 3) {
				throw std::invalid_argument("illegal param count");
			}
			const auto &db = context.getDB();
			const auto tableName = parseTableName(cmd, vparams[0]);
			const auto ptable = db.table(tableName) ?: throw std::invalid_argument("table does not exists");
			const auto &table = *ptable;

			context.setTable(ptable);
			// TODO: check previous rows
			context.setRows(loadRows(table));

			Range columnsRange;
			Range queryRange;

			if (vparams.size() >= 2) {
				Range range(vparams[1]);
				if (isParamRange(cmd, Ranger::lst, range)) {
					columnsRange = range;
				} else {
					queryRange = range;
				}
			}

			if (vparams.size() == 3) { // list selected columns
				Range range(vparams[2]);
				if (columnsRange.isEmpty() && isParamRange(cmd, Ranger::lst, range)) {
					columnsRange = range;
				} else if (queryRange.isEmpty()) {
					queryRange = range;
				} else {
					throw std::invalid_argument("illegal third param type");
				}
			}

			if (!columnsRange.isEmpty()) {
				auto vcols = paramSplit(cmd, Ranger::lst, columnsRange);
				std::vector<ColumnInfo> columns;
				for (auto &colRange : vcols) {
					auto &col = getColumn(context, table, cmd, colRange);
					columns.push_back(col);
				}
				context.setCols(columns);
			} else {
				context.setCols(table.columns);
			}

			if (!queryRange.isEmpty()) { // has query
				auto &res = eval(context, cmd, queryRange);
				if (res.hasError()) {
					return context.err("error in query"); // TODO: SelectResult
				}
				context.setRows(res.getRows(false));
			}

			loadData(table, context.getCols(false), context.getRows(false));

			return context.done();
		}

		Eval (evalAnd) {
			if (vparams.size() < 2) {
				throw std::invalid_argument("illegal param count (at least 2)");
			}
			const auto &table = *(context.getpTable() ?: throw std::invalid_argument("query outside of Select"));

//			// TODO: check previous rows
//			const auto &rows = context.rows();
//
//			// TODO: IMPLEMENT
//			Context *currentContext = &context;
//			for (const auto &vparam: vparams) {
//				//TODO: DELETE OLD
//				currentContext = &eval(*currentContext, cmd, vparam);
//				if (currentContext->hasError()) {
//					return *currentContext;
//				}
//			  // TODO: delete res context
//			}

			return context.todo();
		}

		Eval (evalOr) {
			if (vparams.size() < 2) {
				throw std::invalid_argument("illegal param count (at least 2)");
			}
			const auto &table = *(context.getpTable() ?: throw std::invalid_argument("query outside of Select"));

			// TODO: check previous rows
			const auto &rows = context.getRows();

			std::vector<DataRow> result;
			for (const auto &vparam: vparams) {
				Context &res = eval(context, cmd, vparam);
				if (res.hasError()) {
					return context.err();
				}
				join(result, result, res.getRows(false));
				// TODO: delete res context
			}

			context.setRows(result);
			return context.done();
		}

		Eval (evalNot) {
			if (vparams.size() != 1) {
				throw std::invalid_argument("illegal param count (exactly 1)");
			}
			const auto &table = *(context.getpTable() ?: throw std::invalid_argument("query outside of Select"));

			// TODO: check previous rows
			auto rows = loadRows(table);

			Context &res = eval(context, cmd, vparams[0]);
			if (res.hasError()) {
				return context.err();
			}
			delta(rows, rows, res.getRows(false));
			// TODO: delete res context

			context.setRows(rows);
			return context.done();
		}


	}
}