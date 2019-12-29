#include<string>
#include <set>
#include "../../utils.h"
#include "../../Data/Data.h"
#include "../utils.h"
#include "Eval.h"

namespace db {
	namespace ctx {
		int indexOfDataRow(std::vector<DataRow> &rows, DataRow &target) {
			for (int i = 0; i < rows.size(); i++) {
				//if (rows[i] == target) {
				if (rows[i].unique() == target.unique()) {
					return i;
				}
			}
			return -1;
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
			context.setRows(loadRows(table)); // TODO: check previous rows

			Range columnsRange;
			Range queryRange;

			if (vparams.size() >= 2) {
				Range range(vparams[1]);
				if (isParamRange(cmd, Ranger::lst, range)) {
					columnsRange = range;
				} else  {
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
				auto &queryContext = eval(context, cmd, queryRange);
				if (queryContext.hasError()) {
					return context.err("error in query");
				}
				context.setRows(queryContext.getRows(false));
			}

			loadData(table, context.getCols(false), context.getRows(false));
			return context.done();
		}

		/** NOTE:
		 * first way is calculate each query result and join (same records on all queries) as final result
		 * Faster Way is to implement and ability in queries is to CASCADE each query and pass its
		 * result (rows) to following query and harvest the last query
		 * here I implement second way as it is more preferment and faster
		 */
		Eval (evalAnd) {
			if (vparams.size() < 2) {
				throw std::invalid_argument("illegal param count (at least 2)");
			}
			const auto &table = *(context.getpTable() ?: throw std::invalid_argument("query outside of Select"));

			auto *currentContext = new Context(&context);
			for (const auto &vparam: vparams) {
				auto *newContext = &eval(*currentContext, cmd, vparam);
				// delete currentContext; // TODO: delete me
				currentContext = newContext;
				if (currentContext->hasError()) {
					context.err(currentContext->res(false).message);
					//delete currentContext; // TODO: delete me
					return context;
				}
			}

			context.setRows(currentContext->getRows(false));
			delete currentContext;
			return context.done();
		}

		/** NOTE:
		 * first way is calculate each query result and merge (every records on all queries) as final result
		 * Faster Way is to implement and ability in queries is to CASCADE each query and pass its
		 * result (rows) to following query and harvest the last query
		 * here I implement second way as it is more preferment and faster
		 */
		Eval (evalOr) {
			if (vparams.size() < 2) {
				throw std::invalid_argument("illegal param count (at least 2)");
			}
			const auto &table = *(context.getpTable() ?: throw std::invalid_argument("query outside of Select"));

			std::vector<DataRow> resultRows;
			for (const auto &vparam: vparams) {
				auto &queryContext = eval(context, cmd, vparam);
				if (queryContext.hasError()) {
					context.err(queryContext.res(false).message);
					return context;
				}
				auto &allRows = context.getRows();
				auto &queryRows = queryContext.getRows(false);
				std::vector<DataRow> nextRows;
				for (auto &row: allRows) {
					auto i = indexOfDataRow(queryRows, row);
					if (i == -1) { // not in this query
						nextRows.push_back(row);
					} else {
						// no need to check dups because already delete from previous steps
						resultRows.push_back(row);
					}
				}
				context.setRows(nextRows);
			}

			context.setRows(resultRows);
			return context.done();
		}

		Eval (evalNot) {
			if (vparams.size() != 1) {
				throw std::invalid_argument("illegal param count (exactly 1)");
			}
			const auto &table = *(context.getpTable() ?: throw std::invalid_argument("query outside of Select"));

			auto &queryContext = eval(context, cmd, vparams[0]);
			if (queryContext.hasError()) {
				return context.err(queryContext.res().message);
			}

			std::vector<DataRow> resultRows;
			auto &allRows = context.getRows();
			auto &queryRows = queryContext.getRows(false);
			for (auto &row: allRows) {
				auto i = indexOfDataRow(queryRows, row);
				if (i == -1) { // not in this query
					resultRows.push_back(row);
				}
			}
			context.setRows(resultRows);
			return context.done();
		}
	}
}