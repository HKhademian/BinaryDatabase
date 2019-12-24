#ifndef DATABASE_DATA_TYPE_IO_H
#define DATABASE_DATA_TYPE_IO_H

#include <iostream>
#include "DataType.h"

namespace Database {
	std::ostream &write(std::ostream &os, const void *data, const DataType &type);
	std::istream &read(std::istream &os, void *data, const DataType &type);

	std::ostream &writeBin(std::ostream &os, const void *data, const size_t &size);
	std::istream &readBin(std::istream &is, void *data, const size_t &size);
	std::ostream &writeData(std::ostream &os, const void *data, const DataType &type);
	std::istream &readData(std::istream &is, void *data, const DataType &type);

	std::istream &readByte(std::istream &is, TypeByte &value) ;
	std::istream &readInt(std::istream &is, TypeInt &value) ;
	std::istream &readReal(std::istream &is, TypeReal &value) ;
	std::istream &readType(std::istream &is, DataType &value) ;
	std::istream &readSize(std::istream &is, TypeSize &value) ;
	std::istream &readText(std::istream &is, TypeText &value) ;

	TypeSize readSize(std::istream &is);
	TypeByte readByte(std::istream &is);
	TypeInt readInt(std::istream &is);
	TypeReal readReal(std::istream &is);
	DataType readType(std::istream &is);
	TypeText readText(std::istream &is);

	std::ostream &writeSize(std::ostream &os, const TypeSize &value);
	std::ostream &writeByte(std::ostream &os, const TypeByte &value);
	std::ostream &writeInt(std::ostream &os, const TypeInt &value);
	std::ostream &writeReal(std::ostream &os, const TypeReal &value);
	std::ostream &writeType(std::ostream &os, const DataType &value);
	std::ostream &writeText(std::ostream &os, const TypeText &value);
}
#endif
