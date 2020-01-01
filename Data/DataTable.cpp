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
				if (!row.isFree()) {
					rows.push_back(row);
				}
			} catch (...) {
				break;
			}
		}

		if (columns.empty()) {
			stream.close();
			return rows;
		}

		loadData(stream, columns, rows);
		return rows;
	}

	std::vector<DataRow> loadData(std::istream &stream, const std::vector<ColumnInfo> &columns, const std::vector<RowInfo> &rowInfos) {
		std::vector<DataRow> rows;
		if (rowInfos.empty()) return rows;
		const auto &table = rowInfos[0].table;
		for (auto &rowInfo:rowInfos) {
			DataRow row(rowInfo);
			if (!row.isFree()) {
				rows.push_back(row);
			}
		}
		loadData(stream, columns, rows);
		return rows;
	}

	void removeDataRows(std::vector<RowInfo> &rows) {
		if (rows.empty()) return;
		const auto &table = rows[0].table;
		auto stream = table.openDataOutputStream();
		for (auto &row:rows) {
			row.setFree(true);
			row.writeInfo(stream);
		}
		stream.close();
	}

	void removeDataRows(std::vector<DataRow> &rows) {
		if (rows.empty()) return;
		const auto &table = rows[0].table;
		auto stream = table.openDataOutputStream();
		for (auto &row:rows) {
			row.setFree(true);
			row.writeInfo(stream);
		}
		stream.close();
	}


	std::vector<DataRow> loadRows(const TableInfo &table) {
		return loadRows(table, std::vector<ColumnInfo>());
	}

	void loadData(std::istream &stream, const std::vector<ColumnInfo> &columns, std::vector<DataRow> &rows) {
		for (auto &row:rows) {
			row.readData(stream, columns);
		}
	}

	void loadData(const std::vector<ColumnInfo> &columns, std::vector<DataRow> &rows) {
		if (rows.empty()) return;
		const auto &table = rows[0].table;
		auto stream = table.openDataInputStream();
		loadData(stream, columns, rows);
		stream.close();
	}

	void updateDataRows(const std::vector<ColumnInfo> &columns, std::vector<DataRow> &rows) {
		if (rows.empty()) return;
		const auto &table = rows[0].table;
		auto stream = table.openDataOutputStream();
		for (auto &row:rows) {
			row.writeData(stream, columns);
		}
		stream.close();
	}

}
