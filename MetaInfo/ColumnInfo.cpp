#include <iostream>
#include "DataType.h"
#include "DataTypeIO.h"
#include "ColumnInfo.h"

namespace db {
	std::ostream &operator<<(std::ostream &os, const ColumnInfo &data) {
		writeType(os, data.type);
		writeText(os, data.name);
		return os;
	}

	std::istream &operator>>(std::istream &is, ColumnInfo &data) {
		readType(is, data.type);
		readText(is, data.name);
		return is;
	}

	TypeSize ColumnInfo::getRowSize() const {
		return getTypeSize(type) * getTypeCount(type);
	}

	bool ColumnInfo::operator==(const ColumnInfo &rhs) const {
		return this->type == rhs.type && this->name == rhs.name;
	}

	bool ColumnInfo::operator<(const ColumnInfo &rhs) const {
		return this->type < rhs.type || this->name < rhs.name;
	}
}
