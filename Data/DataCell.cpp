#include <iostream>
#include "../MetaInfo/DataType.h"
#include "DataCell.h"

namespace db {
	DataCell::DataCell(const ColumnInfo &column, const size_t offsetOnRow)
		: DataValue(column.type, false), column(column), size(column.getRowSize()), offsetOnRow(offsetOnRow) {}

	std::ostream &operator<<(std::ostream &os, const DataCell &cell) {
		const auto &type = cell.column.type;
		return writeData(os, cell.getValue(), type);
	}

	std::istream &operator>>(std::istream &is, DataCell &cell) {
		if (isDataType(cell.type(), TYPE_TEXT)) {
			return readData(is, (void *) cell.getValue(), cell.type());
		}
		if (isDataType(cell.type(), TYPE_BYTE)) {
			return readData(is, (void *) cell.getValue(), cell.type());
		}
		if (isDataType(cell.type(), TYPE_INT)) {
			return readData(is, (void *) cell.getValue(), cell.type());
		}
		if (isDataType(cell.type(), TYPE_REAL)) {
			return readData(is, (void *) cell.getValue(), cell.type());
		}
		throw TypeError();
	}

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
		return offsetOnRow != -1;
	}
}
