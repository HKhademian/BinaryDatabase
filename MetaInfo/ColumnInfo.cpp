#include <iostream>
#include "DataType.h"
#include "DataTypeIO.h"
#include "ColumnInfo.h"

namespace db {
	std::ostream &operator<<(std::ostream &os, const ColumnInfo &column) {
		writeType(os, column.type);
		writeText(os, column.name);
		return os;
	}

	std::istream &operator>>(std::istream &is, ColumnInfo &column) {
		readType(is, column.type);
		readText(is, column.name);
		return is;
	}

	TypeSize ColumnInfo::getRowSize() const {
		if (isDataType(type, TYPE_TEXT)) {
			return sizeof(TypeSize);
		}
		return getTypeSize(type) * getTypeCount(type);
	}

	bool ColumnInfo::operator==(const ColumnInfo &rhs) const {
		return this->type == rhs.type && this->name == rhs.name;
	}

	bool ColumnInfo::operator<(const ColumnInfo &rhs) const {
		return this->type < rhs.type || this->name < rhs.name;
	}
}
