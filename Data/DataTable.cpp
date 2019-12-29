#include <fstream>
#include "DataTable.h"

namespace db {
	std::vector<DataRow> loadRows(const TableInfo &table, const std::vector<ColumnInfo> &columns) {
		std::vector<DataRow> rows;

		auto stream = table.openDataInputStream();
		while (stream) {
			DataRow row(table);
			try {
				row.readInfo(stream);
				rows.push_back(row);
			} catch (...) {
				break;
			}
		}

		if (columns.empty()) {
			stream.close();
			return rows;
		}

		loadData(stream, table, columns, rows);
		return rows;
	}

	std::vector<DataRow> loadRows(const TableInfo &table) {
		return loadRows(table, std::vector<ColumnInfo>());
	}


	void loadData(std::istream &stream, const TableInfo &table, const std::vector<ColumnInfo> &columns, std::vector<DataRow> &rows) {
		for (auto &row:rows) {
			row.readData(stream, columns);
		}
	}

	std::vector<DataRow> loadData(std::istream &stream, const TableInfo &table, const std::vector<ColumnInfo> &columns, const std::vector<RowInfo> &rowInfos) {
		std::vector<DataRow> rows;
		for (auto &rowInfo:rowInfos) {
			DataRow row(rowInfo);
			rows.push_back(row);
		}
		loadData(stream, table, columns, rows);
		return rows;
	}

	void loadData(const TableInfo &table, const std::vector<ColumnInfo> &columns, std::vector<DataRow> &rows) {
		auto stream = table.openDataInputStream();
		loadData(stream, table, columns, rows);
		stream.close();
	}


	void updateDataRows(const TableInfo &table, const std::vector<ColumnInfo> &columns, std::vector<DataRow> &rows) {
		auto stream = table.openDataOutputStream();
		for (auto &row:rows) {
			row.writeData(stream, columns);
		}
		stream.close();
	}

	void removeDataRows(const TableInfo &table, std::vector<RowInfo> &rows) {
		auto stream = table.openDataOutputStream();
		for (auto &row:rows) {
			row.setFree(true);
			row.writeInfo(stream);
		}
		stream.close();
	}

	void removeDataRow(RowInfo &row) {
		row.setFree(true);
		row.writeInfo();
	}
}
