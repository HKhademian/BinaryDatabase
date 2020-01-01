#include <cstring> // memcpy
#include "DataValue.h"
#include "../MetaInfo/DataType.h"
#include "../utils.h"

namespace db {
	DataValue::~DataValue() {
		reset();
	}

	DataValue::DataValue(const DataType &dataType, bool typeChange)
		: dataType(dataType), typeChange(typeChange) {
		allocSpace();
		clear(); // fill with zero
	}

	DataValue::DataValue(const DataValue &other)
		: DataValue(other.dataType, other.typeChange) {}

	bool DataValue::operator<(const DataValue &rhs) const { return self.compare(rhs) < 0; }

	bool DataValue::operator>(const DataValue &rhs) const { return self.compare(rhs) > 0; }

	bool DataValue::operator==(const DataValue &rhs) const { return self.compare(rhs) == 0; }

	std::ostream &operator<<(std::ostream &stream, const DataValue &val) {
		if (!val.hasVal) return stream << "NON";
		if (val.value == nullptr) return stream << "NULL";
		if (isDataType(val.dataType, TYPE_TEXT)) {
			return stream << *(TypeText *) val.value;
		}
		if (isDataType(val.dataType, TYPE_BYTE)) {
			return stream << (int) *(TypeByte *) val.value;
		}
		if (isDataType(val.dataType, TYPE_INT)) {
			return stream << *(TypeInt *) val.value;
		}
		if (isDataType(val.dataType, TYPE_REAL)) {
			return stream << *(TypeReal *) val.value;
		}
		throw TypeError();
	}


	DataType DataValue::type() const {
		return dataType;
	}

	DataValue &DataValue::type(const DataType &newtype) {
		if (dataType == newtype) return self;
		if (!typeChange) throw std::logic_error("Cannot change dataType");
		reset();
		dataType = newtype;
		return allocSpace();
	}

	DataValue &DataValue::allocSpace() {
		if (value) return self;
		const auto &count = getTypeCount(dataType);
		if (isDataType(dataType, TYPE_TEXT)) {
			value = new TypeText;
			return self;
		}
		if (isDataType(dataType, TYPE_BYTE)) {
			value = new TypeByte[count];
			return self;
		}
		if (isDataType(dataType, TYPE_INT)) {
			value = new TypeInt[count];
			return self;
		}
		if (isDataType(dataType, TYPE_REAL)) {
			value = new TypeReal[count];
			return self;
		}
		throw TypeError();
	}

	DataValue &DataValue::reset() {
		if (isDataType(dataType, TYPE_TEXT)) {
			delete (TypeText *) value;
			value = nullptr;
			return self;
		}
		if (isDataType(dataType, TYPE_BYTE)) {
			delete[] (TypeByte *) value;
			value = nullptr;
			return self;
		}
		if (isDataType(dataType, TYPE_INT)) {
			delete[] (TypeInt *) value;
			value = nullptr;
			return self;
		}
		if (isDataType(dataType, TYPE_REAL)) {
			delete[] (TypeReal *) value;
			value = nullptr;
			return self;
		}
		throw TypeError();
	}

	DataValue &DataValue::clear() {
		hasVal = false;
		if (value == nullptr) { return self; }

		const auto &count = getTypeCount(dataType);
		if (isDataType(dataType, TYPE_TEXT)) {
			(*(TypeText *) value).clear();
			return self;
		}
		if (isDataType(dataType, TYPE_BYTE)) {
			std::fill_n((TypeByte *) value, count, (TypeByte) 0);
			return self;
		}
		if (isDataType(dataType, TYPE_INT)) {
			std::fill_n((TypeInt *) value, count, (TypeInt) 0);
			return self;
		}
		if (isDataType(dataType, TYPE_REAL)) {
			std::fill_n((TypeReal *) value, count, (TypeReal) 0);
			return self;
		}
		throw TypeError();
	}

	bool DataValue::isInited() const {
		return value != nullptr;
	}

	bool DataValue::isTypeVar() const {
		return isDataTypeVar(dataType);
	}

	bool DataValue::isDataVar() const {
		return !hasVal && isDataTypeVar(dataType);
	}

	bool DataValue::hasValue() const {
		return hasVal;
	}

	TypeSize DataValue::getCount() const {
		return getTypeCount(dataType);
	}

	TypeSize DataValue::getSize() const {
		if (isDataVar()) {
			return -1;
		}
		if (isDataType(dataType, TYPE_TEXT)) {
			return sizeof(TypeSize) + sizeof(TypeByte) * (*(TypeText *) value).size();
		}
		return getTypeSize(dataType);
	}

	const void *DataValue::getValue() const {
		return value;
	}

	const TypeByte *DataValue::getByte(const size_t pos) const {
		if (value == nullptr)
			throw std::logic_error("value not init");
		if (!isDataType(dataType, TYPE_BYTE))
			throw TypeError(TypeError::MSG_MISMATCH);
		if (pos >= getTypeCount(dataType))
			throw std::out_of_range("pos");
		return ((TypeByte *) value) + pos;
	}

	const TypeInt *DataValue::getInt(const size_t pos) const {
		if (value == nullptr)
			throw std::logic_error("value not init");
		if (!isDataType(dataType, TYPE_INT))
			throw TypeError(TypeError::MSG_MISMATCH);
		if (pos >= getTypeCount(dataType))
			throw std::out_of_range("pos");
		return ((TypeInt *) value) + pos;
	}

	const TypeReal *DataValue::getReal(const size_t pos) const {
		if (value == nullptr)
			throw std::logic_error("value not init");
		if (!isDataType(dataType, TYPE_REAL))
			throw TypeError(TypeError::MSG_MISMATCH);
		if (pos >= getTypeCount(dataType))
			throw std::out_of_range("pos");
		return ((TypeReal *) value) + pos;
	}

	const TypeText *DataValue::getText() const {
		if (value == nullptr)
			throw std::logic_error("value not init");
		if (!isDataType(dataType, TYPE_TEXT))
			throw TypeError(TypeError::MSG_MISMATCH);
		return (TypeText *) value;
	}

	DataValue &DataValue::setValue(const void *source, const DataType &typ, const size_t count) {
		if (!typeChange && !isDataType(dataType, typ))
			throw TypeError(TypeError::MSG_MISMATCH);
		type(typ); //REMINDER: may change the holding value type
		allocSpace(); //FIXME: unnecessary
		clear(); //REMINDER: if not above happened make sure to clear all previous data

		const auto &typecount = getTypeCount(dataType);
		const auto &cnt = typecount < count ? typecount : count;

		hasVal = true;
		if (isDataType(dataType, TYPE_TEXT)) {
			*(TypeText *) value = *(TypeText *) source;
			return self;
		}
		if (isDataType(dataType, TYPE_BYTE)) {
			memcpy(value, source, sizeof(TypeByte) * cnt);
			return self;
		}
		if (isDataType(dataType, TYPE_INT)) {
			memcpy(value, source, sizeof(TypeInt) * cnt);
			return self;
		}
		if (isDataType(dataType, TYPE_REAL)) {
			memcpy(value, source, sizeof(TypeReal) * cnt);
			return self;
		}
		hasVal = false;
		throw TypeError();
	}

	DataValue &DataValue::setValue(const void *val, const size_t count) {
		return setValue(val, dataType, count);
	}

	DataValue &DataValue::setValue(const DataValue &from) {
		return setValue(from.value, from.dataType, getTypeCount(from.dataType));
	}


	DataValue &DataValue::setValueByte(const TypeByte *val, const size_t count) {
		return setValue(val, TYPE_BYTE, count);
	}

	DataValue &DataValue::setValueByte(const TypeByte &val) {
		return setValue(&val, TYPE_BYTE, 1);
	}


	DataValue &DataValue::setValueInt(const TypeInt *val, const size_t count) {
		return setValue(val, TYPE_INT, count);
	}

	DataValue &DataValue::setValueInt(const TypeInt &val) {
		return setValue(&val, TYPE_INT, 1);
	}


	DataValue &DataValue::setValueReal(const TypeReal *val, const size_t count) {
		return setValue(val, TYPE_REAL, count);
	}

	DataValue &DataValue::setValueReal(const TypeReal &val) {
		return setValue(&val, TYPE_REAL, 1);
	}


	DataValue &DataValue::setValueText(const TypeText *val) {
		return setValue(val, TYPE_TEXT, 1);
	}

	DataValue &DataValue::setValueText(const TypeText &val) {
		return setValue(&val, TYPE_TEXT, 1);
	}

	int DataValue::compare(const DataValue &other) const {
		if (&other == this)
			return 0; // reference compare
		if (&other == nullptr)
			return 1; // reference compare null safety

		const auto *pl = !self.hasVal ? nullptr : self.getValue();
		const auto *pr = !other.hasVal ? nullptr : other.getValue();
		if (pl == nullptr)
			return pr == nullptr ? 0 : -1;
		if (pr == nullptr)
			return 1;

		if (isDataType(self.dataType, TYPE_TEXT)) {
			if (isDataType(other.dataType, TYPE_TEXT)) {
				const auto &l = *(TypeText *) pl, &r = *(TypeText *) pr;
				return l.compare(r);
			}
		}
		if (isDataType(self.dataType, TYPE_BYTE)) {
			if (isDataType(other.dataType, TYPE_BYTE)) {
				// TODO: check arrays
				const auto &l = *(TypeByte *) pl, &r = *(TypeByte *) pr;
				return l > r ? 1 : l < r ? -1 : 0;
			}
		}
		if (isDataType(self.dataType, TYPE_INT)) {
			if (isDataType(other.dataType, TYPE_INT)) {
				// TODO: check arrays
				const auto &l = *(TypeInt *) pl, &r = *(TypeInt *) pr;
				return l > r ? 1 : l < r ? -1 : 0;
			}
		}
		if (isDataType(self.dataType, TYPE_REAL)) {
			if (isDataType(other.dataType, TYPE_REAL)) {
				// TODO: check arrays
				const auto &l = *(TypeReal *) pl, &r = *(TypeReal *) pr;
				return l > r ? 1 : l < r ? -1 : 0;
			}
		}
		throw TypeError();
	}


}