#ifndef DATABASE_DATA_CELL_H
#define DATABASE_DATA_CELL_H

#include <iostream>
#include "../MetaInfo/DataType.h"
#include "../MetaInfo/DataTypeIO.h"
#include "../MetaInfo/ColumnInfo.h"

namespace db {
	struct DataRow;

	class DataCell {
		void *value = nullptr;

	public:
		const ColumnInfo &column;
		const size_t size = 0;
		size_t offsetOnRow = -1;

		explicit DataCell(const ColumnInfo &column, size_t offsetOnRow = -1);

		~ DataCell();

//		explicit DataCell(const ColumnInfo &col, const DataType &type, const void *val) :
//			DataCell(col) {
//			setValueText(val, type);
//		}
//
//		explicit DataCell(const ColumnInfo &col, const void *val) :
//			DataCell(col) {
//			setValueText(val);
//		}
//
//		explicit DataCell(const ColumnInfo &col, const TypeByte &val) :
//			DataCell(col) {
//			setValueText(val);
//		}
//
//		explicit DataCell(const ColumnInfo &col, const TypeInt &val) :
//			DataCell(col) {
//			setValueText(val);
//		}
//
//		explicit DataCell(const ColumnInfo &col, const TypeReal &val) :
//			DataCell(col) {
//			setValueText(val);
//		}
//
//		explicit DataCell(const ColumnInfo &col, const TypeText &val) :
//			DataCell(col) {
//			setValueText(val);
//		}

		friend std::ostream &operator<<(std::ostream &os, const DataCell &self);

		friend std::istream &operator>>(std::istream &is, DataCell &self);

		void setValue(const void *source, const DataType &type);

		void setValue(const void *val);

		void setValueByte(const TypeByte &val);

		void setValueInt(const TypeInt &val);

		void setValueReal(const TypeReal &val);

		void setValueText(const TypeText &val);

		const void *getValue() const;

		const TypeByte *getByte(size_t pos = 0) const;

		const TypeInt *getInt(size_t pos = 0) const;

		const TypeReal *getReal(size_t pos = 0) const;

		const TypeText *getText() const;

		size_t getRowSize() const;

		/// reset to not inited data and free storage
		void reset();

		/// set data to zero
		void clearValue();

		/// has any data or it's not init
		bool hasData();

		/// has static position on row or need to calculate
		bool hasOffset();

	private:
		void allocData();

		void freeData();
	};
}
#endif
