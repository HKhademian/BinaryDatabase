#ifndef DATABASE_DATA_CELL_H
#define DATABASE_DATA_CELL_H

#include <iostream>
#include "DataValue.h"
#include "../MetaInfo/MetaInfo.h"

namespace db {
	struct DataCell : public DataValue {
		const ColumnInfo &column;
		const size_t staticSize = 0;
		size_t offsetOnDisk = -1;

		explicit DataCell(const ColumnInfo &column, size_t offsetOnRow);

	public:
		size_t getRowSize() const;

		/// has static position on row or need to calculate
		bool hasOffset();

		/// has any data
		bool hasData();

	public: // R/W
		std::istream &seekg(std::istream &stream) const;

		std::ostream &seekp(std::ostream &stream) const;

		std::istream &readData(std::istream &stream);

		std::ostream &writeData(std::ostream &stream);
	};
}
#endif
