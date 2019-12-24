#ifndef DATABASE_DATA_ROW_H
#define DATABASE_DATA_ROW_H

#include <map>
#include "TableInfo.h"
#include "DataCell.h"

namespace Database {
	struct DataRow {
		explicit DataRow(const TableInfo &info);

	private:
		const TableInfo &tableInfo;
		std::map<std::string, DataCell*> cells;

	public:
		size_t getRowSize() const;

		DataCell& atColumn(const ColumnInfo &column) const;

		friend std::istream &operator>>(std::istream &is, DataRow &self);

		friend std::ostream &operator<<(std::ostream &os, const DataRow &self);
	};
}

#endif