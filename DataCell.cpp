#include <iostream>
#include "DataType.h"
#include "DataCell.h"
#include "utils.h"

namespace Database {
	DataCell::DataCell(const ColumnInfo &col) : columnInfo(col) {

	}



	std::ostream &operator<<(std::ostream &os, const DataCell &self) {
		const auto &type = self.columnInfo.type;
		return writeData(os, self.value, type);
	}

	std::istream &operator>>(std::istream &is, DataCell &self) {
		self.freeValue();

		const auto &type = self.columnInfo.type;
		const auto &staticCount = getTypeCount(type);

		if (isDataType(type, TYPE_TEXT)) {
			self.value = new TypeText;
			return readData(is, self.value, type);
		}

		if (isDataType(type, TYPE_BYTE)) {
			self.value = new TypeByte[staticCount];
			return readData(is, self.value, type);
		}

		if (isDataType(type, TYPE_INT)) {
			self.value = new TypeInt[staticCount];
			return readData(is, self.value, type);
		}

		if (isDataType(type, TYPE_REAL)) {
			self.value = new TypeReal[staticCount];
			return readData(is, self.value, type);
		}

		throw UnsupportedType();
	}


	const void *DataCell::setValue(const void *val, const DataType &type) {
		if (!isDataType(columnInfo.type, type))
			throw UnsupportedType();

		freeValue();

		if (isDataType(type, TYPE_TEXT)) {
			value = val == nullptr ? nullptr : new TypeText(*(TypeText *) val);
			return value;
		}
		if (isDataType(type, TYPE_BYTE)) {
			value = val == nullptr ? nullptr : new TypeByte(*(TypeByte *) val);
			return value;
		}
		if (isDataType(type, TYPE_INT)) {
			value = val == nullptr ? nullptr : new TypeInt(*(TypeInt *) val);
			return value;
		}
		if (isDataType(type, TYPE_REAL)) {
			value = val == nullptr ? nullptr : new TypeReal(*(TypeReal *) val);
			return value;
		}

		throw UnsupportedType();
	}

	const void *DataCell::setValue(const void *val) {
		return setValue(val, columnInfo.type);
	}

	const void *DataCell::setValue(const TypeByte &val) {
		return setValue(&val, TYPE_INT);
	}

	const void *DataCell::setValue(const TypeInt &val) {
		return setValue(&val, TYPE_INT);
	}

	const void *DataCell::setValue(const TypeReal &val) {
		return setValue(&val, TYPE_REAL);
	}

	const void *DataCell::setValue(const TypeText &val) {
		return setValue((void *) &val, TYPE_TEXT);
	}

	size_t DataCell::getRowSize() const {
		const auto &type = columnInfo.type;
		if (isDataType(type, TYPE_TEXT)) {
			return sizeof(TypeSize) + (*(TypeText *) value).size();
		}

		const auto &count = getTypeCount(type);
		if (isDataType(type, TYPE_BYTE)) {
			return sizeof(TypeByte) * count;
		}
		if (isDataType(type, TYPE_INT)) {
			return sizeof(TypeInt) * count;
		}
		if (isDataType(type, TYPE_REAL)) {
			return sizeof(TypeReal) * count;
		}
		if (isDataType(type, TYPE_SIZE)) {
			return sizeof(TypeSize) * count;
		}
		throw UnsupportedType();
	}

	void DataCell::freeValue() {
		const auto &type = columnInfo.type;
		if (isDataType(type, TYPE_TEXT)) {
			delete (TypeText *) value;
			return;
		}
		if (isDataType(type, TYPE_BYTE)) {
			delete[] (TypeByte *) value;
			return;
		}
		if (isDataType(type, TYPE_INT)) {
			delete[] (TypeInt *) value;
			return;
		}
		if (isDataType(type, TYPE_REAL)) {
			delete[] (TypeReal *) value;
			return;
		}
		throw UnsupportedType();
	}

	void DataCell::clearValue() {
		freeValue();
		value = nullptr;
	}


	const void *DataCell::getValue() const {
		return value;
	}

	const TypeByte *DataCell::getByte() const {
		if (isDataType(columnInfo.type, TYPE_BYTE)) {
			return (TypeByte *) value;
		}
		throw UnsupportedType();
	}

	const TypeInt *DataCell::getInt() const {
		if (isDataType(columnInfo.type, TYPE_INT)) {
			return (TypeInt *) value;
		}
		throw UnsupportedType();
	}

	const TypeReal *DataCell::getReal() const {
		if (isDataType(columnInfo.type, TYPE_REAL)) {
			return (TypeReal *) value;
		}
		throw UnsupportedType();
	}

	const TypeText *DataCell::getText() const {
		if (isDataType(columnInfo.type, TYPE_TEXT))
			return (TypeText *) value;
		throw UnsupportedType();
	}


}
