#ifndef DATABASE_TABLE_INFO_H
#define DATABASE_TABLE_INFO_H

#include <vector>
#include "ColumnInfo.h"

namespace db {
	struct TableInfo {
		std::string name = "";
		std::vector<ColumnInfo> columns;

		TableInfo() = default;

		explicit TableInfo(std::string nam) :
			name(std::move(nam)) {}

		friend std::ostream &operator<<(std::ostream &os, const TableInfo &data);

		friend std::istream &operator>>(std::istream &is, TableInfo &data);

		size_t getRowSize() const;

		size_t getDataOffset(int index) const;

		int column(const std::string &columnName) const;

		std::string getDataFilePath() const {
			return "./tbl-" + name + ".dat";
		}
	};
}
#endif
