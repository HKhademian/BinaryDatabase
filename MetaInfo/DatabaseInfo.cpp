#include <iostream>
#include "../utils.h"
#include "TableInfo.h"
#include "DatabaseInfo.h"
#include "DataTypeIO.h"

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

		const TypeSize count = readSize(is);
		loop (i, count) {
			auto table = TableInfo(); //auto tablePos = *new TableInfo();
			is >> table;
			data.tables.push_back(table);
		}

		return is;
	}

	int DatabaseInfo::tablePos(const std::string &tableName) const {
		loop(i, tables.size()) {
			if (strcaseequal(tables[i].name, tableName)) {
				return i;
			}
		}
		return -1;
	}

	const TableInfo *DatabaseInfo::table(const std::string &tableName) const {
		for (const auto &table : tables) {
			if (strcaseequal(table.name, tableName)) {
				return &table;
			}
		}
		return nullptr;
	}

}
