#ifndef DATABASE_DATA_ROW_H
#define DATABASE_DATA_ROW_H

#include <map>
#include "../MetaInfo/TableInfo.h"
#include "DataCell.h"

namespace db {
	TypeFlag rowFlag(bool free);

	bool isRowFree(TypeFlag flag);

	class DataRow {
	protected:
		TypeFlag flag = rowFlag(true);
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

		std::istream &readInfo(std::istream &is);

		std::istream &readData(std::istream &is, const std::vector<ColumnInfo> &columns);
	};
}

#endif