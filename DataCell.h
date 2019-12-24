#ifndef DATABASE_DATA_CELL_H
#define DATABASE_DATA_CELL_H

#include <iostream>
#include "DataType.h"
#include "DataTypeIO.h"
#include "ColumnInfo.h"

namespace Database {
	struct DataCell {
		explicit DataCell(const ColumnInfo &col);

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

		const void *setValue(const void *val, const DataType &type);

		const void *setValue(const void *val);

		const void *setValue(const TypeByte &val);

		const void *setValue(const TypeInt &val);

		const void *setValue(const TypeReal &val);

		const void *setValue(const TypeText &val);

		const void *getValue() const;

		const TypeByte *getByte() const;

		const TypeInt *getInt() const;

		const TypeReal *getReal() const;

		const TypeText *getText() const;

		void freeValue();

		void clearValue();

		size_t getRowSize() const;
	};
}
#endif
