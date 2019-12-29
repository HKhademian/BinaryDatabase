#ifndef DATABASE_DATA_TABLE_H
#define DATABASE_DATA_TABLE_H

#include "../MetaInfo/TableInfo.h"
#include "DataRow.h"

namespace db {
	std::vector<DataRow> loadRows(const TableInfo &table);

	std::vector<DataRow> loadRows(const TableInfo &table, const std::vector<ColumnInfo> &columns);


	void loadData(std::istream &stream, const TableInfo &table, const std::vector<ColumnInfo> &columns, std::vector<DataRow> &rows);

	std::vector<DataRow> loadData(std::istream &stream, const TableInfo &table, const std::vector<ColumnInfo> &columns, const std::vector<RowInfo> &rowInfos);

	void loadData(const TableInfo &table, const std::vector<ColumnInfo> &columns, std::vector<DataRow> &rows);


	void updateDataRows(const TableInfo &table, const std::vector<ColumnInfo> &columns, std::vector<DataRow> &rows);

	void removeDataRows(const TableInfo &table, std::vector<RowInfo> &rows);
}

#endif
