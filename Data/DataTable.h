#ifndef DATABASE_DATA_TABLE_H
#define DATABASE_DATA_TABLE_H

#include "../MetaInfo/TableInfo.h"
#include "DataRow.h"

namespace db {
	std::vector<DataRow> loadRows(const TableInfo &table);

	std::vector<DataRow> loadRows(const TableInfo &table, const std::vector<ColumnInfo> &columns);

	std::vector<DataRow> loadData(const TableInfo &table, const std::vector<ColumnInfo> &columns, std::vector<DataRow> &rows);

	std::vector<DataRow> loadData(std::ifstream &is, const TableInfo &table, std::vector<DataRow> &rows);

	std::vector<DataRow> loadData(std::ifstream &is, const TableInfo &table, std::vector<DataRow> &rows, const std::vector<ColumnInfo> &columns);

	void insertData(DataRow &row);
}

#endif
