#include "DataValue.h"
#include "../MetaInfo/DataType.h"
#include "../utils.h"

namespace db {
	DataValue::~DataValue() {
		reset();
	}

	DataValue::DataValue(const DataType &type) : type(type) {
		allocSpace();
	}

	bool DataValue::operator<(const DataValue &rhs) const { return self.compare(rhs) < 0; }

	bool DataValue::operator>(const DataValue &rhs) const { return self.compare(rhs) > 0; }

	bool DataValue::operator==(const DataValue &rhs) const { return self.compare(rhs) == 0; }

	DataValue &DataValue::allocSpace() {
		if (value) return self;
		const auto &count = getTypeCount(type);
		if (isDataType(type, TYPE_TEXT)) {
			value = new TypeText;
			return self;
		}
		if (isDataType(type, TYPE_BYTE)) {
			value = new TypeByte[count];
			return self;
		}
		if (isDataType(type, TYPE_INT)) {
			value = new TypeInt[count];
			return self;
		}
		if (isDataType(type, TYPE_REAL)) {
			value = new TypeReal[count];
			return self;
		}
		throw TypeError();
	}

	DataValue &DataValue::reset() {
		if (isDataType(type, TYPE_TEXT)) {
			delete (TypeText *) value;
			return self;
		}
		if (isDataType(type, TYPE_BYTE)) {
			delete[] (TypeByte *) value;
			return self;
		}
		if (isDataType(type, TYPE_INT)) {
			delete[] (TypeInt *) value;
			return self;
		}
		if (isDataType(type, TYPE_REAL)) {
			delete[] (TypeReal *) value;
			return self;
		}
		throw TypeError();
	}

	DataValue &DataValue::clear() {
		hasVal = false;
		if (value == nullptr) { return self; }

		const auto &count = getTypeCount(type);
		if (isDataType(type, TYPE_TEXT)) {
			(*(TypeText *) value).clear();
			return self;
		}
		if (isDataType(type, TYPE_BYTE)) {
			std::fill_n((TypeByte *) value, count, (TypeByte) 0);
			return self;
		}
		if (isDataType(type, TYPE_INT)) {
			std::fill_n((TypeInt *) value, count, (TypeInt) 0);
			return self;
		}
		if (isDataType(type, TYPE_REAL)) {
			std::fill_n((TypeReal *) value, count, (TypeReal) 0);
			return self;
		}
		throw TypeError();
	}

	bool DataValue::isInited() const {
		return value != nullptr;
	}

	bool DataValue::hasValue() const {
		return hasVal;
	}

	TypeSize DataValue::getCount() const {
		return getTypeCount(type);
	}

	TypeSize DataValue::getSize() const {
		if (isDataType(type, TYPE_TEXT)) {
			return sizeof(TypeSize) + sizeof(TypeByte) * (*(TypeText *) value).size();
		}
		return getTypeSize(type);
	}

	const void *DataValue::getValue() const {
		return value;
	}

	const TypeByte *DataValue::getByte(const size_t pos) const {
		if (value == nullptr)
			throw std::logic_error("value not init");
		if (!isDataType(type, TYPE_BYTE))
			throw TypeError(TypeError::MSG_MISMATCH);
		if (pos >= getTypeCount(type))
			throw std::out_of_range("pos");
		return ((TypeByte *) value) + pos;
	}

	const TypeInt *DataValue::getInt(const size_t pos) const {
		if (value == nullptr)
			throw std::logic_error("value not init");
		if (!isDataType(type, TYPE_INT))
			throw TypeError(TypeError::MSG_MISMATCH);
		if (pos >= getTypeCount(type))
			throw std::out_of_range("pos");
		return ((TypeInt *) value) + pos;
	}

	const TypeReal *DataValue::getReal(const size_t pos) const {
		if (value == nullptr)
			throw std::logic_error("value not init");
		if (!isDataType(type, TYPE_REAL))
			throw TypeError(TypeError::MSG_MISMATCH);
		if (pos >= getTypeCount(type))
			throw std::out_of_range("pos");
		return ((TypeReal *) value) + pos;
	}

	const TypeText *DataValue::getText() const {
		if (value == nullptr)
			throw std::logic_error("value not init");
		if (!isDataType(type, TYPE_TEXT))
			throw TypeError(TypeError::MSG_MISMATCH);
		return (TypeText *) value;
	}

	DataValue &DataValue::setValue(const void *source, const DataType &typ, const size_t count) {
		if (!isDataType(type, typ))
			throw TypeError(TypeError::MSG_MISMATCH);
		allocSpace();
		clear();
		const auto &typcount = getTypeCount(type);
		const auto &cnt = typcount < count ? typcount : count;

		hasVal = true;
		if (isDataType(type, TYPE_TEXT)) {
			*(TypeText *) value = *(TypeText *) source;
			return self;
		}
		if (isDataType(type, TYPE_BYTE)) {
			memcpy(value, source, sizeof(TypeByte) * cnt);
			return self;
		}
		if (isDataType(type, TYPE_INT)) {
			memcpy(value, source, sizeof(TypeInt) * cnt);
			return self;
		}
		if (isDataType(type, TYPE_REAL)) {
			memcpy(value, source, sizeof(TypeReal) * cnt);
			return self;
		}
		hasVal = false;
		throw TypeError();
	}


	DataValue &DataValue::setValue(const void *val, const size_t count) {
		return setValue(val, type, count);
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
		if (&other == this) return 0; // reference compare
		if (&other == nullptr) return 1; // reference compare null safety

		const auto *pl = self.getValue();
		const auto *pr = other.getValue();
		if (pl == nullptr) return pr == nullptr ? 0 : -1;
		if (pr == nullptr) return 1;

		if (isDataType(type, TYPE_TEXT)) {
			const auto &vl = *(TypeText *) pl, &vr = *(TypeText *) pr;
			return vl.compare(vr);
		}
		if (isDataType(type, TYPE_BYTE)) {
			// TODO: check arrays
			const auto &l = *(TypeText *) pl, &r = *(TypeText *) pr;
			return l.compare(r);
		}
		if (isDataType(type, TYPE_INT)) {
			// TODO: check arrays
			const auto &l = *(TypeInt *) pl, &r = *(TypeInt *) pr;
			return l - r;
		}
		if (isDataType(type, TYPE_REAL)) {
			// TODO: check arrays
			const auto &l = *(TypeReal *) pl, &r = *(TypeReal *) pr;
			return l > r ? 1 : l < r ? -1 : 0;
		}
		throw TypeError();
	}


}