#ifndef DATABASE_DATA_CELL_H
#define DATABASE_DATA_CELL_H

#include <iostream>
#include "../MetaInfo/DataType.h"
#include "../MetaInfo/DataTypeIO.h"
#include "../MetaInfo/ColumnInfo.h"

namespace db {
	struct DataCell {
		explicit DataCell(const ColumnInfo &col);
		~ DataCell();

		explicit DataCell(const ColumnInfo &col, const DataType &type, const void *val) :
				DataCell(col) {
			setValue(val, type);
		}

		explicit DataCell(const ColumnInfo &col, const void *val) :
				DataCell(col) {
			setValue(val);
		}

		explicit DataCell(const ColumnInfo &col, const TypeByte &val) :
				DataCell(col) {
			setValue(val);
		}

		explicit DataCell(const ColumnInfo &col, const TypeInt &val) :
				DataCell(col) {
			setValue(val);
		}

		explicit DataCell(const ColumnInfo &col, const TypeReal &val) :
				DataCell(col) {
			setValue(val);
		}

		explicit DataCell(const ColumnInfo &col, const TypeText &val) :
				DataCell(col) {
			setValue(val);
		}

	private:
		const ColumnInfo &columnInfo;
		void *value = nullptr;

	public:
		friend std::ostream &operator<<(std::ostream &os, const DataCell &self);

		friend std::istream &operator>>(std::istream &is, DataCell &self);

		void setValue(const void *val, const DataType &type);

		void setValue(const void *val);

		void setValue(const TypeByte &val);

		void setValue(const TypeInt &val);

		void setValue(const TypeReal &val);

		void setValue(const TypeText &val);

		const void *getValue() const;

		const TypeByte *getByte(size_t pos = 0) const;

		const TypeInt *getInt(size_t pos = 0) const;

		const TypeReal *getReal(size_t pos = 0) const;

		const TypeText *getText() const;

		void clearValue();

		size_t getRowSize() const;
	};
}
#endif
