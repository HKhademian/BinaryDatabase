#include <iostream>
#include "../MetaInfo/DataType.h"
#include "DataCell.h"
#include "DataRow.h"

namespace db {
	DataCell::DataCell(const ColumnInfo &column, size_t offsetOnDisk) :
		DataValue(column.type, false),
		column(column),
		staticSize(column.getRowSize()),
		offsetOnDisk(offsetOnDisk) {}

	size_t DataCell::getRowSize() const {
		if (isDataType(type(), TYPE_TEXT)) {
			return sizeof(TypeSize) + (*(TypeText *) getValue()).size();
		}
		return getTypeSize(type());
	}

	bool DataCell::hasData() {
		return hasValue();
	}

	bool DataCell::hasOffset() {
		return offsetOnDisk != -1;
	}

	std::ostream &DataCell::seekp(std::ostream &stream) const {
		return stream.seekp(offsetOnDisk, std::ostream::beg);
	}

	std::istream &DataCell::seekg(std::istream &stream) const {
		return stream.seekg(offsetOnDisk, std::ostream::beg);
	}

	std::ostream &DataCell::writeData(std::ostream &stream) {
		if (!hasValue()) return stream; // do not write no-value cells
		seekp(stream);
		return DataValue::write(stream);
	}

	std::istream &DataCell::readData(std::istream &stream) {
		seekg(stream);
		return DataValue::read(stream);
	}
}
