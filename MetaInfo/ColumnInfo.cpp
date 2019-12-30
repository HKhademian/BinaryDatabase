#include <iostream>
#include "../utils.h"
#include "DataType.h"
#include "DataTypeIO.h"
#include "ColumnInfo.h"

namespace db {
	std::ostream &ColumnInfo::writeInfo(std::ostream &stream) const {
		writeType(stream, self.type);
		writeText(stream, self.name);
		return stream;
	}

	std::istream &ColumnInfo::readInfo(std::istream &stream) {
		readType(stream, self.type);
		readText(stream, self.name);
		return stream;
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
