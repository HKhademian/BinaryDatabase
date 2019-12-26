#ifndef DATABASE_DATATYPE_H
#define DATABASE_DATATYPE_H

#include <cstdint>
#include <iostream>
#include <cstdio>

namespace db {
	typedef uint32_t DataType;
	typedef uint32_t TypeSize;
	typedef int8_t TypeByte;
	typedef int32_t TypeInt;
	typedef float TypeReal;
	typedef std::string TypeText;
	typedef uint16_t TypeFlag;

	static const auto TYPE_MASK = (DataType) 0b11;
	static const auto TYPE_BYTE = (DataType) 0b00;
	static const auto TYPE_INT = (DataType) 0b01;
	static const auto TYPE_REAL = (DataType) 0b10;
	static const auto TYPE_TEXT = (DataType) 0b11;
	static const auto TYPE_SIZE = TYPE_INT;

	struct TypeError : public std::invalid_argument {
		constexpr static const auto MSG_MISMATCH = "source mismatch target type";
		constexpr static const auto MSG_UNSUPPORTED = "Unsupported Type";

		TypeError() : TypeError(MSG_UNSUPPORTED) {}

		explicit TypeError(const char *msg) : std::invalid_argument(msg) {}

		explicit TypeError(const std::string &msg) : std::invalid_argument(msg) {}
	};

	DataType dataType(const DataType &type, const TypeSize &count = 1);

	TypeSize getTypeCount(const DataType &type);

	TypeSize getTypeSize(const DataType &type);

	bool isDataType(const DataType &type, const DataType &target);
}

#endif
