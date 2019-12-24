#ifndef DATABASE_DATABASE_INFO_H
#define DATABASE_DATABASE_INFO_H

#include "TableInfo.h"

namespace Database {
	struct DatabaseInfo {
		std::string name ="";
		std::vector<TableInfo> tables;

		DatabaseInfo() = default;

		explicit DatabaseInfo(std::string nam) :
				name(std::move(nam)) {}

		friend std::ostream &operator<<(std::ostream &os, const DatabaseInfo &data);

		friend std::istream &operator>>(std::istream &is, DatabaseInfo &data);
	};

}
#endif
