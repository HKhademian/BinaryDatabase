#ifndef DATABASE_DATA_ROW_H
#define DATABASE_DATA_ROW_H

#include <map>
#include "../MetaInfo/TableInfo.h"
#include "../MetaInfo/RowInfo.h"

namespace db {
	struct DataCell;

	struct DataRow : public RowInfo {
	private:
		std::vector<DataCell *> cells;

	public:
		~DataRow();

		explicit DataRow(const TableInfo &table);

		explicit DataRow(const RowInfo &rowInfo);

		size_t getRowSize() const;

		/// clear all cells data
		DataRow &clear();

		DataCell *atColumn(const std::string &colName) const;

		DataCell *atColumn(const ColumnInfo &column) const;


		std::istream &readData(std::istream &is, const std::vector<ColumnInfo> &columns);

		std::ostream &writeData(std::ostream &stream, const std::vector<ColumnInfo> &columns);

	private:
		DataRow &calculateCellsOffset();

	};
}

#endif