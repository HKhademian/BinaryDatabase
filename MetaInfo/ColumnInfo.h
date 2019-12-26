#ifndef DATABASE_COLUMN_INFO_H
#define DATABASE_COLUMN_INFO_H

#include <iostream>
#include "DataType.h"

namespace db {
	struct ColumnInfo {
		DataType type = 0;
		std::string name = "";

		ColumnInfo() = default;

		explicit ColumnInfo(const DataType &typ, std::string nam) :
				type(typ), name(std::move(nam)) {}

		friend std::ostream &operator<<(std::ostream &os, const ColumnInfo &self);

		friend std::istream &operator>>(std::istream &is, ColumnInfo &self);

		bool operator==(const ColumnInfo &rhs) const;
		bool operator <(const ColumnInfo& rhs) const;

		size_t getRowSize() const;
	};
}

#endif
