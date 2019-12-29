#include "DataType.h"

namespace db {
	TypeSize getTypeCount(const DataType &type) {
		return (type >> 2) + 1;
	}

	TypeSize getTypeSize(const DataType &type) {
		const auto len = getTypeCount(type);
		return len * (
			isDataType(type, TYPE_BYTE) ? sizeof(TypeByte) :
			isDataType(type, TYPE_INT) ? sizeof(TypeInt) :
			isDataType(type, TYPE_REAL) ? sizeof(TypeReal) :
			isDataType(type, TYPE_TEXT) ? 0 :
			throw TypeError()
		);
	}

	DataType dataType(const DataType &type, const TypeSize &count) {
		return (type & TYPE_MASK) | ((count - 1) << 2);
	}

	bool isDataType(const DataType &type, const DataType &target) {
		return (type & TYPE_MASK) == (target & TYPE_MASK);
	}

	bool isDataTypeVar(const DataType &type) {
		return isDataType(type, TYPE_TEXT);
	}
}
