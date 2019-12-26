#include <iostream>
#include "../utils.h"
#include "../MetaInfo/DataType.h"
#include "DataCell.h"

namespace db {
	DataCell::DataCell(const ColumnInfo &column, const size_t offsetOnRow)
		: column(column), size(column.getRowSize()), offsetOnRow(offsetOnRow) {}

	DataCell::~DataCell() {
		reset();
	}

	void DataCell::allocData() {
		if (value) return;
		const auto &type = column.type;
		const auto &count = getTypeCount(type);
		if (isDataType(type, TYPE_TEXT)) {
			value = new TypeText;
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

	void DataCell::freeData() {
		if (!value) return;
		const auto &type = column.type;
		const auto &count = getTypeCount(type);
		if (isDataType(type, TYPE_TEXT)) {
			delete (TypeText *) value;
		} else if (isDataType(type, TYPE_BYTE)) {
			delete[] (TypeByte *) value;
		} else if (isDataType(type, TYPE_INT)) {
			delete[] (TypeInt *) value;
		} else if (isDataType(type, TYPE_REAL)) {
			delete[] (TypeReal *) value;
		} else {
			throw TypeError();
		}
	}


	std::ostream &operator<<(std::ostream &os, const DataCell &self) {
		const auto &type = self.column.type;
		return writeData(os, self.value, type);
	}

	std::istream &operator>>(std::istream &is, DataCell &self) {
		const auto &type = self.column.type;
		const auto &staticCount = getTypeCount(type);

		self.allocData();
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

	void DataCell::setValue(const void *source, const DataType &type) {
		if (!isDataType(column.type, type))
			throw TypeError(TypeError::MSG_MISMATCH);

		allocData();
		const auto &count = getTypeCount(type);
		if (isDataType(type, TYPE_TEXT)) {
			*(TypeText *) value = *(TypeText *) source;
		} else if (isDataType(type, TYPE_BYTE)) {
			memcpy(value, source, sizeof(TypeByte) * count);
		} else if (isDataType(type, TYPE_INT)) {
			memcpy(value, source, sizeof(TypeInt) * count);
		} else if (isDataType(type, TYPE_REAL)) {
			memcpy(value, source, sizeof(TypeReal) * count);
		} else {
			throw TypeError();
		}
	}

	void DataCell::setValue(const void *val) {
		return setValue(val, column.type);
	}

	void DataCell::setValueByte(const TypeByte &val) {
		return setValue(&val, TYPE_BYTE);
	}

	void DataCell::setValueInt(const TypeInt &val) {
		return setValue(&val, TYPE_INT);
	}

	void DataCell::setValueReal(const TypeReal &val) {
		return setValue(&val, TYPE_REAL);
	}

	void DataCell::setValueText(const TypeText &val) {
		return setValue(&val, TYPE_TEXT);
	}

	size_t DataCell::getRowSize() const {
		const auto &type = column.type;
		if (isDataType(type, TYPE_TEXT)) {
			return sizeof(TypeSize) + (*(TypeText *) value).size();
		}
		return getTypeSize(type);
	}

	void DataCell::clearValue() {
		if (value == nullptr)return;
		const auto &type = column.type;
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

	void DataCell::reset() {
		freeData();
	}

	const void *DataCell::getValue() const {
		return value;
	}

	const TypeByte *DataCell::getByte(size_t pos) const {
		if (!isDataType(column.type, TYPE_BYTE))
			throw TypeError(TypeError::MSG_MISMATCH);
		if (pos >= getTypeCount(column.type))
			throw std::out_of_range("pos");
		return (TypeByte *) value + pos;
	}

	const TypeInt *DataCell::getInt(size_t pos) const {
		if (!isDataType(column.type, TYPE_INT))
			throw TypeError(TypeError::MSG_MISMATCH);
		if (pos >= getTypeCount(column.type))
			throw std::out_of_range("pos");
		return (TypeInt *) value + pos;
	}

	const TypeReal *DataCell::getReal(size_t pos) const {
		if (!isDataType(column.type, TYPE_REAL))
			throw TypeError(TypeError::MSG_MISMATCH);
		if (pos >= getTypeCount(column.type))
			throw std::out_of_range("pos");
		return (TypeReal *) value + pos;
	}

	const TypeText *DataCell::getText() const {
		if (!isDataType(column.type, TYPE_TEXT))
			throw TypeError(TypeError::MSG_MISMATCH);
		return (TypeText *) value;
	}


	bool DataCell::hasData() {
		return value != nullptr;
	}

	bool DataCell::hasOffset() {
		return offsetOnRow != -1;
	}


}
