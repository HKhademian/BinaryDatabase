#include<string>
#include "../../MetaInfo/TableInfo.h"
#include "../../Data/DataTable.h"
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

		Eval (evalAnd) {
			if (vparams.size() < 2) {
				throw std::invalid_argument("illegal param count (at least 2)");
			}
			const auto &table = *(context.table ?: throw std::invalid_argument("query outside of Select"));

			// TODO: check previous rows
			auto rows = loadRows(table);
			context.rows = new std::vector<DataRow>(rows);

			Context *currentContext = &context;
			for (const auto &vparam: vparams) {
				currentContext = eval(*currentContext, cmd, vparam);
				if (currentContext->hasError()) {
					return currentContext;
				}
			}

			return eval(context, cmd, vparams[1]);
		}

		Eval (evalOr) {
			if (vparams.size() < 2) {
				throw std::invalid_argument("illegal param count (at least 2)");
			}
			const auto &table = *(context.table ?: throw std::invalid_argument("query outside of Select"));

			// TODO: check previous rows
			auto rows = loadRows(table);
			context.rows = &rows;

			auto &sum = *new std::vector<DataRow>();
			for (const auto &vparam: vparams) {
				Context *result = eval(context, cmd, vparam);
				if (result->hasError()) {
					return result;
				}
				//TODO: add(sum, result->rows);
			}

			context.rows = &sum;
			return context.done();
		}

		Eval (evalNot) {
			if (vparams.size() != 1) {
				throw std::invalid_argument("illegal param count (exactly 1)");
			}
			const auto &table = *(context.table ?: throw std::invalid_argument("query outside of Select"));

			// TODO: check previous rows
			auto rows = loadRows(table);
			context.rows = new std::vector<DataRow>(rows);

			Context *result = eval(context, cmd, vparams[1]);
			if (result->hasError()) {
				return result;
			}
			//TODO: remove(rows, result->rows);

			return context.done();
		}


	}
}