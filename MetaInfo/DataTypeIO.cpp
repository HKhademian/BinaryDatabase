#include "../utils.h"
#include "DataTypeIO.h"

namespace db {
	std::ostream &writeBin(std::ostream &os, const void *data, const size_t &size) {
		static const char zeros[100]{0};
		if (os.write(data == nullptr ? zeros : (char *) data, size)) {
			return os;
		}
		throw std::out_of_range("stream is ended.");
	}

	std::istream &readBin(std::istream &is, void *data, const size_t &size) {
		if (data == nullptr) { // skip data
			if (!is.seekg(size, std::istream::cur)) {
				return is;
			}
		} else if (is.read((char *) data, size)) {
			return is;
		}
		throw std::out_of_range("stream is ended.");
	}

	std::ostream &writeData(std::ostream &os, const void *data, const DataType &type) {
		if (isDataType(type, TYPE_TEXT)) {
			const TypeText &value = *(TypeText *) data;
			writeSize(os, value.size());
			return os << value;
		}

		const auto typeCount = getTypeCount(type);
		const size_t size =
			isDataType(type, TYPE_BYTE) ? sizeof(TypeByte) :
			isDataType(type, TYPE_INT) ? sizeof(TypeInt) :
			isDataType(type, TYPE_REAL) ? sizeof(TypeReal) :
			throw TypeError();
		return writeBin(os, data, size * typeCount);
	}

	std::istream &readData(std::istream &is, void *data, const DataType &type) {
		if (isDataType(type, TYPE_TEXT)) {
			const auto len = readSize(is);
			TypeText &value = *(TypeText *) data;
			value.clear();
			if (len <= 0) {
				return is;
			}
			value = TypeText(len, '\0');
			return readBin(is, &value[0], len);
		}

		const auto count = getTypeCount(type);
		const size_t size =
			isDataType(type, TYPE_BYTE) ? sizeof(TypeByte) :
			isDataType(type, TYPE_INT) ? sizeof(TypeInt) :
			isDataType(type, TYPE_REAL) ? sizeof(TypeReal) :
			throw TypeError();
		return readBin(is, data, size * count);
	}


	std::istream &readByte(std::istream &is, TypeByte &value) {
		return readBin(is, &value, sizeof(TypeByte));
	}

	std::istream &readInt(std::istream &is, TypeInt &value) {
		return readBin(is, &value, sizeof(TypeInt));
	}

	std::istream &readReal(std::istream &is, TypeReal &value) {
		return readBin(is, &value, sizeof(TypeReal));
	}

	std::istream &readType(std::istream &is, DataType &value) {
		return readBin(is, &value, sizeof(DataType));
	}

	std::istream &readSize(std::istream &is, TypeSize &value) {
		return readBin(is, &value, sizeof(TypeSize));
	}

	std::istream &readFlag(std::istream &is, TypeFlag &value) {
		return readBin(is, &value, sizeof(TypeFlag));
	}


	TypeByte readByte(std::istream &is) {
		TypeByte val;
		readByte(is, val);
		return val;
	}

	TypeInt readInt(std::istream &is) {
		TypeInt val;
		readInt(is, val);
		return val;
	}

	TypeReal readReal(std::istream &is) {
		TypeReal val;
		readReal(is, val);
		return val;
	}

	TypeSize readSize(std::istream &is) {
		TypeSize val;
		readSize(is, val);
		return val;
	}

	DataType readType(std::istream &is) {
		DataType val;
		readType(is, val);
		return val;
	}

	TypeFlag readFlag(std::istream &is) {
		TypeFlag val;
		readFlag(is, val);
		return val;
	}

	std::ostream &writeByte(std::ostream &os, const TypeByte &value) {
		return writeBin(os, &value, sizeof(TypeByte));
	}

	std::ostream &writeInt(std::ostream &os, const TypeInt &value) {
		return writeBin(os, &value, sizeof(TypeInt));
	}

	std::ostream &writeReal(std::ostream &os, const TypeReal &value) {
		return writeBin(os, &value, sizeof(TypeReal));
	}

	std::ostream &writeSize(std::ostream &os, const TypeSize &value) {
		return writeBin(os, &value, sizeof(TypeSize));
	}

	std::ostream &writeType(std::ostream &os, const DataType &value) {
		return writeBin(os, &value, sizeof(DataType));
	}

	std::ostream &writeFlag(std::ostream &os, const TypeFlag &value) {
		return writeBin(os, &value, sizeof(TypeFlag));
	}


	std::istream &readText(std::istream &is, TypeText &value) {
		return readData(is, &value, TYPE_TEXT);
	}

	std::ostream &writeText(std::ostream &os, const TypeText &value) {
		return writeData(os, &value, TYPE_TEXT);
	}

	TypeText readText(std::istream &is) {
		TypeText val;
		readText(is, val);
		return val;
	}
}
