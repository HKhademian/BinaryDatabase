#ifndef DATABASE_DATA_CELL_H
#define DATABASE_DATA_CELL_H

#include <iostream>
#include "DataValue.h"
#include "../MetaInfo/DataType.h"
#include "../MetaInfo/DataTypeIO.h"
#include "../MetaInfo/ColumnInfo.h"

namespace db {
	struct DataCell : public DataValue {
		const ColumnInfo &column;
		const size_t size = 0;
		size_t offsetOnRow = -1;

		explicit DataCell(const ColumnInfo &column, size_t offsetOnRow = -1);

		friend std::ostream &operator<<(std::ostream &os, const DataCell &cell);

		friend std::istream &operator>>(std::istream &is, DataCell &cell);

		size_t getRowSize() const;

		/// has static position on row or need to calculate
		bool hasOffset();

		/// has any data
		bool hasData();
	};
}
#endif
