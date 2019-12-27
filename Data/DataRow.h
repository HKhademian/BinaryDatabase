#ifndef DATABASE_DATA_ROW_H
#define DATABASE_DATA_ROW_H

#include <map>
#include "../MetaInfo/TableInfo.h"
#include "DataCell.h"

namespace db {
	void rowFlag(TypeFlag &flag, bool free);

	TypeFlag rowFlag(bool free);

	bool isRowFree(TypeFlag flag);

	class DataRow {
	protected:
		TypeFlag flag = rowFlag(false);
		size_t sizeOnDisk = 0;
		std::map<std::string, DataCell *> cells;

	public:
		const TableInfo &table;
		size_t offset = -1;

		explicit DataRow(const TableInfo &info);

		size_t getRowSize() const;

		DataCell &atColumn(const ColumnInfo &column) const;

		friend std::istream &operator>>(std::istream &is, DataRow &row);

		friend std::ostream &operator<<(std::ostream &os, DataRow &row);

		std::istream &readInfo(std::istream &is);

		std::istream &readData(std::istream &is, const std::vector<ColumnInfo> &columns);

		bool isFree() const;

		void setFree(bool isFree);

		size_t unique() const;

	};
}

#endif