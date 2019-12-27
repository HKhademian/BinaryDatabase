#ifndef DATABASE_DATABASE_INFO_H
#define DATABASE_DATABASE_INFO_H

#include "TableInfo.h"

namespace db {
	struct DatabaseInfo {
		TypeText name = "";
		TypeSize version = 1;
		std::vector<TableInfo> tables;

		DatabaseInfo() = default;

		explicit DatabaseInfo(std::string nam) :
			name(std::move(nam)) {}

		friend std::ostream &operator<<(std::ostream &os, const DatabaseInfo &data);

		friend std::istream &operator>>(std::istream &is, DatabaseInfo &data);

		int tablePos(const std::string &tableName) const;

		const TableInfo *table(const std::string &tableName) const;
	};

}
#endif
