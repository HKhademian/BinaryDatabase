#include <iostream>
#include "../utils.h"
#include "../MetaInfo/DataType.h"
#include "DataCell.h"

namespace db {
	DataCell::DataCell(const ColumnInfo &col) : columnInfo(col) {
		const auto &type = col.type;
		const auto &count = getTypeCount(type);
		if (isDataType(type, TYPE_TEXT)) {
			value = new TypeText();
		} else if (isDataType(type, TYPE_BYTE)) {
			value = new TypeByte[count];
		} else if (isDataType(type, TYPE_INT)) {
			value = new TypeInt[count];
		} else if (isDataType(type, TYPE_REAL)) {
			value = new TypeReal[count];
		} else {
			throw TypeError();
		}
	}

	DataCell::~DataCell() {
		const auto &type = columnInfo.type;
		if (isDataType(type, TYPE_TEXT)) {
			delete (TypeText *) value;
		} else if (isDataType(type, TYPE_BYTE)) {
			delete[] (TypeByte *) value;
		} else if (isDataType(type, TYPE_INT)) {
			delete[] (TypeInt *) value;
		} else if (isDataType(type, TYPE_REAL)) {
			delete[] (TypeReal *) value;
		}
	}


	std::ostream &operator<<(std::ostream &os, const DataCell &self) {
		const auto &type = self.columnInfo.type;
		return writeData(os, self.value, type);
	}

	std::istream &operator>>(std::istream &is, DataCell &self) {
		const auto &type = self.columnInfo.type;
		const auto &staticCount = getTypeCount(type);

		if (isDataType(type, TYPE_TEXT)) {
			return readData(is, self.value, type);
		}
		if (isDataType(type, TYPE_BYTE)) {
			return readData(is, self.value, type);
		}
		if (isDataType(type, TYPE_INT)) {
			return readData(is, self.value, type);
		}
		if (isDataType(type, TYPE_REAL)) {
			return readData(is, self.value, type);
		}
		throw TypeError();
	}


	void DataCell::setValue(const void *val, const DataType &type) {
		if (!isDataType(columnInfo.type, type))
			throw TypeError(TypeError::MSG_MISMATCH);

		clearValue();

		// TODO: cpy all array
		if (isDataType(type, TYPE_TEXT)) {
			*(TypeText *) value = *(TypeText *) val;
		} else if (isDataType(type, TYPE_BYTE)) {
			*(TypeByte *) value = *(TypeByte *) val;
		} else if (isDataType(type, TYPE_INT)) {
			*(TypeInt *) value = *(TypeInt *) val;
		} else if (isDataType(type, TYPE_REAL)) {
			*(TypeReal *) value = *(TypeReal *) val;
		} else {
			throw TypeError();
		}
	}

	void DataCell::setValue(const void *val) {
		return setValue(val, columnInfo.type);
	}

	void DataCell::setValue(const TypeByte &val) {
		return setValue(&val, TYPE_BYTE);
	}

	void DataCell::setValue(const TypeInt &val) {
		return setValue(&val, TYPE_INT);
	}

	void DataCell::setValue(const TypeReal &val) {
		return setValue(&val, TYPE_REAL);
	}

	void DataCell::setValue(const TypeText &val) {
		return setValue(&val, TYPE_TEXT);
	}

	size_t DataCell::getRowSize() const {
		const auto &type = columnInfo.type;
		if (isDataType(type, TYPE_TEXT)) {
			return sizeof(TypeSize) + (*(TypeText *) value).size();
		}
		return getTypeSize(type);
	}

	void DataCell::clearValue() {
		const auto &type = columnInfo.type;
		const auto &count = getTypeCount(type);
		if (isDataType(type, TYPE_TEXT)) {
			(*(TypeText *) value).clear();
		} else if (isDataType(type, TYPE_BYTE)) {
			loop(i, count) ((TypeByte *) value)[i] = 0;
		} else if (isDataType(type, TYPE_INT)) {
			loop(i, count) ((TypeInt *) value)[i] = 0;
		} else if (isDataType(type, TYPE_REAL)) {
			loop(i, count) ((TypeReal *) value)[i] = 0;
		} else if (isDataType(type, TYPE_SIZE)) {
			loop(i, count) ((TypeSize *) value)[i] = 0;
		} else {
			throw TypeError();
		}
	}


	const void *DataCell::getValue() const {
		return value;
	}

	const TypeByte *DataCell::getByte(size_t pos) const {
		if (!isDataType(columnInfo.type, TYPE_BYTE))
			throw TypeError(TypeError::MSG_MISMATCH);
		if (pos >= getTypeCount(columnInfo.type))
			throw std::out_of_range("pos");
		return (TypeByte *) value + pos;
	}

	const TypeInt *DataCell::getInt(size_t pos) const {
		if (!isDataType(columnInfo.type, TYPE_INT))
			throw TypeError(TypeError::MSG_MISMATCH);
		if (pos >= getTypeCount(columnInfo.type))
			throw std::out_of_range("pos");
		return (TypeInt *) value + pos;
	}

	const TypeReal *DataCell::getReal(size_t pos) const {
		if (!isDataType(columnInfo.type, TYPE_REAL))
			throw TypeError(TypeError::MSG_MISMATCH);
		if (pos >= getTypeCount(columnInfo.type))
			throw std::out_of_range("pos");
		return (TypeReal *) value + pos;
	}

	const TypeText *DataCell::getText() const {
		if (!isDataType(columnInfo.type, TYPE_TEXT))
			throw TypeError(TypeError::MSG_MISMATCH);
		return (TypeText *) value;
	}


}
