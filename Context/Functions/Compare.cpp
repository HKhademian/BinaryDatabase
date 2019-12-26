#include<string>
#include "../../utils.h"
#include "Eval.h"
#include "Functions.h"

namespace db {
	namespace ctx {
		int compare(const void *pa, const void *pb) {
			//TODO: compare based on type
			return 0;
		};

		int cmpEQ(const void *pa, const void *pb) { return compare(pa, pb) == 0; }

		int cmpNE(const void *pa, const void *pb) { return compare(pa, pb) != 0; }

		int cmpGT(const void *pa, const void *pb) { return compare(pa, pb) > 0; }

		int cmpGE(const void *pa, const void *pb) { return compare(pa, pb) >= 0; }

		int cmpLT(const void *pa, const void *pb) { return compare(pa, pb) < 0; }

		int cmpLE(const void *pa, const void *pb) { return compare(pa, pb) <= 0; }

		Eval (evalCompare) {
			if (context.table == nullptr) {
				throw std::invalid_argument("query outside of Select");
			}
			if (vparams.size() != 2) {
				throw std::invalid_argument("illegal param count");
			}

			const auto &tableInfo = *context.table;
			const auto &records = context.records;
			const auto &column = getColumn(context, tableInfo, cmd, vparams[0]);

			const auto cmp =
				strcaseequal(func, "eq") ? cmpEQ :
				strcaseequal(func, "ne") || strcaseequal(func, "neq") ? cmpNE :
				strcaseequal(func, "gt") || strcaseequal(func, "nle") ? cmpGT :
				strcaseequal(func, "ge") || strcaseequal(func, "nlt") ? cmpGE :
				strcaseequal(func, "lt") || strcaseequal(func, "nge") ? cmpLT :
				strcaseequal(func, "le") || strcaseequal(func, "nlt") ? cmpLE :
				throw std::invalid_argument("invalid compare");

			std::vector<size_t> result;
			for (size_t i = records.size() - 1; i >= 0; i--) {
				//TODO: read data
				const void *cellData = nullptr;
				const void *targetData = nullptr;
				if (cmp(cellData, targetData)) {
					result.push_back(records[i]);
				}
			}

			//TODO: SelectResult
			return context.done();
		}
	}
}