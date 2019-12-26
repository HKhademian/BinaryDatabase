#include <fstream>
#include "DataTable.h"

namespace db {
	std::vector<DataRow> loadData(std::ifstream &is, const TableInfo &table, std::vector<DataRow> &rows, const std::vector<ColumnInfo> &columns) {
		for (auto &row:rows) {
			row.readData(is, columns);
		}
		return rows;
	}

	std::vector<DataRow> loadData(std::ifstream &is, const TableInfo &table, std::vector<DataRow> &rows) {
		return loadData(is, table, rows, table.columns);
	}

	std::vector<DataRow> loadData(const TableInfo &table, const std::vector<ColumnInfo> &columns, std::vector<DataRow> &rows) {
		std::ifstream is;
		is.open(table.getDataFilePath(), std::ios::in);
		is.seekg(std::ifstream::beg);
		loadData(is, table, rows, columns);
		is.close();
		return rows;
	}

	std::vector<DataRow> loadRows(const TableInfo &table, const std::vector<ColumnInfo> &columns) {
		std::vector<DataRow> rows;

		std::ifstream is;
		is.open(table.getDataFilePath(), std::ios::in);
		is.seekg(std::ifstream::beg);

		while (is.good()) {
			DataRow row(table);
			try {
				row.readInfo(is);
				rows.push_back(row);
			} catch (...) {
				break;
			}
		}

		if (columns.empty()) {
			is.close();
			return rows;
		}

		return loadData(is, table, rows, columns);
	}

	std::vector<DataRow> loadRows(const TableInfo &table) {
		return loadRows(table, std::vector<ColumnInfo>());
	}

	void insert(const DataRow &row) {
		//TODO: implement
		throw std::logic_error("not implemented yet!");
	}
}
