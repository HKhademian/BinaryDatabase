#ifndef DATABASE_DATATYPE_H
#define DATABASE_DATATYPE_H

#include <cstdio>

namespace Database {
	typedef uint32_t DataType;
	typedef uint32_t TypeSize;
	typedef int8_t TypeByte;
	typedef int32_t TypeInt;
	typedef float TypeReal;
	typedef std::string TypeText;

	static const auto TYPE_MASK = (DataType) 0b11;
	static const auto TYPE_TEXT = (DataType) 0b01;
	static const auto TYPE_INT =  (DataType) 0b00;
	static const auto TYPE_REAL = (DataType) 0b10;
	static const auto TYPE_BYTE = (DataType) 0b11;
	static const auto TYPE_SIZE = TYPE_INT;

	struct UnsupportedType : public std::invalid_argument {
		UnsupportedType() : UnsupportedType("Unsupported Type") {}

		explicit UnsupportedType(const char *msg) : std::invalid_argument(msg) {}

		explicit UnsupportedType(const std::string &msg) : std::invalid_argument(msg) {}
	};

	DataType dataType(const DataType &type, const TypeSize &count = 1);

	TypeSize getTypeCount(const DataType &type);

	TypeSize getTypeSize(const DataType &type);

	bool isDataType(const DataType &type, const DataType &target);

//	bool isDataVariable(const DataType &type);

}

#endif
