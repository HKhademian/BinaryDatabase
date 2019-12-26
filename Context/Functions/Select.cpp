#include<string>
#include "../../MetaInfo/TableInfo.h"
#include "../../Data/DataTable.h"
#include "Eval.h"
#include "Functions.h"

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
			for (const auto &elt:target) {
				bool exists = false;
				for (const auto &els : source) {
					if (elt.unique() == els.unique()) {
						exists = true;
						break;
					}
				}
				if (!exists) {
					result.push_back(elt);
				}
			}
			return result;
		}


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
			auto rows = context.rows ? *context.rows : loadRows(table);
			context.rows = context.rows ?: new std::vector<DataRow>(rows);

			return eval(context, cmd, vparams[1]);
		}

		Eval (evalAnd) {
			if (vparams.size() < 2) {
				throw std::invalid_argument("illegal param count (at least 2)");
			}
			const auto &table = *(context.table ?: throw std::invalid_argument("query outside of Select"));

			// TODO: check previous rows
			auto rows = context.rows ? *context.rows : loadRows(table);
			context.rows = context.rows ?: new std::vector<DataRow>(rows);

			Context *currentContext = &context;
			for (const auto &vparam: vparams) {
				//TODO: DELETE OLD
				currentContext = eval(*currentContext, cmd, vparam);
				if (currentContext->hasError()) {
					return currentContext;
				}
			}

			return currentContext->done();
		}

		Eval (evalOr) {
			if (vparams.size() < 2) {
				throw std::invalid_argument("illegal param count (at least 2)");
			}
			const auto &table = *(context.table ?: throw std::invalid_argument("query outside of Select"));

			// TODO: check previous rows
			auto rows = context.rows ? *context.rows : loadRows(table);
			context.rows = &rows;

			auto &result = *new std::vector<DataRow>();
			for (const auto &vparam: vparams) {
				Context *res = eval(context, cmd, vparam);
				if (res->hasError()) {
					return res;
				}
				join(result, result, *res->rows);
			}

			context.rows = &result;
			return context.done();
		}

		Eval (evalNot) {
			if (vparams.size() != 1) {
				throw std::invalid_argument("illegal param count (exactly 1)");
			}
			const auto &table = *(context.table ?: throw std::invalid_argument("query outside of Select"));

			// TODO: check previous rows
			auto rows = context.rows ? *context.rows : loadRows(table);
			context.rows = &rows;

			Context *res = eval(context, cmd, vparams[1]);
			if (res->hasError()) {
				return res;
			}

			context.rows = new std::vector<DataRow>;
			delta(*context.rows, rows, *res->rows);

			return context.done();
		}


	}
}