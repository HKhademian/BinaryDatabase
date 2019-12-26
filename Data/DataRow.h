#ifndef DATABASE_DATA_ROW_H
#define DATABASE_DATA_ROW_H

#include <map>
#include "../MetaInfo/TableInfo.h"
#include "DataCell.h"

namespace db {
	TypeFlag rowflag(bool free);
	bool isRowFree(TypeFlag rowflag);

	class DataRow {
	protected:
		TypeFlag flag = rowflag(true);
		size_t sizeOnDisk = -1;
		size_t offset = -1;
		std::map<std::string, DataCell *> cells;

	public:
		const TableInfo &table;

		explicit DataRow(const TableInfo &info);

		size_t getRowSize() const;

		DataCell &atColumn(const ColumnInfo &column) const;

		friend std::istream &operator>>(std::istream &is, DataRow &self);

		friend std::ostream &operator<<(std::ostream &os, DataRow &self);
	};
}

#endif