#include <fstream>
#include "DataTable.h"

namespace db {
	std::vector<DataRow> loadData(std::ifstream &is, const TableInfo &table, std::vector<DataRow> &rows, const std::vector<ColumnInfo> &columns) {
		for (auto &row:rows) {
			is.seekg(row.offset, std::ifstream::beg);
			row.readData(is, columns);
		}
		return rows;
	}

	std::vector<DataRow> loadData(std::ifstream &is, const TableInfo &table, std::vector<DataRow> &rows) {
		return loadData(is, table, rows, table.columns);
	}

	std::vector<DataRow> loadData(const TableInfo &table, const std::vector<ColumnInfo> &columns, std::vector<DataRow> &rows) {
		std::ifstream is;
		is.open(table.getDataPath(), std::ios::in | std::ios::out | std::ios::binary);
		is.seekg(0, std::ifstream::beg);
		loadData(is, table, rows, columns);
		is.close();
		return rows;
	}

	std::vector<DataRow> loadRows(const TableInfo &table, const std::vector<ColumnInfo> &columns) {
		std::vector<DataRow> rows;

		std::ifstream is;
		is.open(table.getDataPath(), std::ios::in | std::ios::out | std::ios::binary);
		is.seekg(0, std::ifstream::beg);

		while (is) {
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

	void insertData(DataRow &row) {
		const auto &path = row.table.getDataPath();
		std::ofstream os;
		os.open(path, std::ios::in | std::ios::out | std::ios::binary);
		if (!os) {
			os.open(path, std::ios::out | std::ios::binary);
		}
		os << row;
		os.close();
	}
}
