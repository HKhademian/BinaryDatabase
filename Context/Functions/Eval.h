#ifndef DATABASE_CONTEXT_EVAL_H
#define DATABASE_CONTEXT_EVAL_H

#include <string>
#include "../params.h"
#include "../Result.h"
#include "../Context.h"

namespace db {
	namespace ctx {
#define Eval(evalFunc) Result evalFunc (Context &context, const std::string &func, const std::string &cmd, const std::vector<Range>& vparams)
#define EvalConst(evalFunc) Result evalFunc (const Context &context, const std::string &func, const std::string &cmd, const std::vector<Range>& vparams)

		typedef Eval ((*EvalP));

		typedef EvalConst ((*EvalConstP));

		Eval (evalCreateTable);

		Eval (evalDeleteTable);

		Eval (evalSelect);

		Eval (evalCompare);

		Eval (evalInsert);

		Eval (evalRemove);

	}
}
#endif
