#include<string>
#include "../../Database.h"
#include "../../utils.h"
#include "../utils.h"
#include "Eval.h"

namespace db {
	namespace ctx {
		EvalP chooseFunction(const std::string &cmd, Range &range, std::string &func) {
			static const std::string CMD_CREATE_TABLE = "CreateTable";
			static const std::string CMD_DELETE_TABLE = "DeleteTable";
			static const std::string CMD_INSERT = "Insert";
			static const std::string CMD_UPDATE = "Update";
			static const std::string CMD_REMOVE = "Remove";
			static const std::string CMD_SELECT = "Select";
			static const std::string CMD_AND = "And";
			static const std::string CMD_OR = "Or";
			static const std::string CMD_NOT = "Not";

			paramTrim(cmd, range);
			Range paramRange(range);
			if (paramFindRange(cmd, Ranger::func, paramRange)) {
				Range funcRange(range.start, paramRange.start - 1);
				func = paramSub(cmd, funcRange);
				if (strcaseequal(func, CMD_CREATE_TABLE)) {
					return &evalCreateTable;
				}
				if (strcaseequal(func, CMD_DELETE_TABLE)) {
					return &evalDeleteTable;
				}

				if (strcaseequal(func, CMD_INSERT)) {
					return &evalInsert;
				}
				if (strcaseequal(func, CMD_UPDATE)) {
					return &evalUpdate;
				}
				if (strcaseequal(func, CMD_REMOVE)) {
					return &evalRemove;
				}

				if (strcaseequal(func, CMD_SELECT)) {
					return &evalSelect;
				}
				if (strcaseequal(func, CMD_AND)) {
					return &evalAnd;
				}
				if (strcaseequal(func, CMD_OR)) {
					return &evalOr;
				}
				if (strcaseequal(func, CMD_NOT)) {
					return &evalNot;
				}

				if (strcaseequal(func, "eq") || strcaseequal(func, "neq") || strcaseequal(func, "ne") ||
				    strcaseequal(func, "gt") || strcaseequal(func, "nle") || strcaseequal(func, "ge") ||
				    strcaseequal(func, "nlt") || strcaseequal(func, "lt") || strcaseequal(func, "nge") ||
				    strcaseequal(func, "le") || strcaseequal(func, "nlt")) {
					return &evalCompare;
				}
			}
			return nullptr;
		}

		Context &eval(Context &context, const std::string &cmd, Range range) {
			auto newContext = new Context(&context);
			//TODO: track delete pointers
			return snapEval(*newContext, cmd, range);
		}

		Context &snapEval(Context &context, const std::string &cmd, Range range) {
			std::string func;

			EvalP function = chooseFunction(cmd, range, func);
			if (function != nullptr) {
				const auto vparam = paramSplit(cmd, Ranger::func, range);
				try {
					return (*function)(context, func, cmd, vparam);
				} catch (std::exception &err) {
					return context.err(err.what());
				}
			}

			return context.err("function not found");
		}

	}
}
