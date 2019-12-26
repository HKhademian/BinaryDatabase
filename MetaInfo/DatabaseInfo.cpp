#include <iostream>
#include "DataTypeIO.h"
#include "TableInfo.h"
#include "DatabaseInfo.h"
#include "../utils.h"

namespace db {
	std::ostream &operator<<(std::ostream &os, const DatabaseInfo &data) {
		writeText(os, data.name);

		const auto count = data.tables.size();
		writeSize(os, count);

		loop (i, count) {
			os << data.tables[i];
		}

		return os;
	}

	std::istream &operator>>(std::istream &is, DatabaseInfo &data) {
		data.tables.clear();

		readText(is, data.name);

		const size_t count = readSize(is);
		loop (i, count) {
			auto el = new TableInfo();
			is >> *el;
			data.tables.push_back(*el);
		}

		return is;
	}

	int DatabaseInfo::table(const std::string &tableName) const {
		loop(i, tables.size()) {
			if (strcaseequal(tables[i].name, tableName)) {
				return i;
			}
		}
		return -1;
	}

}
