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

		std::ostream &writeInfo(std::ostream &stream) const;

		std::istream &readInfo(std::istream &stream);

		bool operator==(const ColumnInfo &rhs) const;
		bool operator <(const ColumnInfo& rhs) const;

		TypeSize getRowSize() const;
	};
}

#endif
