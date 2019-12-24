#include "DataCell.h"
#include "DataRow.h"
#include "utils.h"

namespace Database {
	DataRow::DataRow(const TableInfo &info) :
			tableInfo(info) {
		for (const auto &col : tableInfo.columns) {
			cells.insert(std::pair<std::string, DataCell *>(col.name, new DataCell(col)));
		}
	}

	std::istream &operator>>(std::istream &is, DataRow &self) {
		for (auto &cell : self.cells) {
			cell.second->clearValue();
		}

		const size_t dataSize = readSize(is);
		if (dataSize <= 0) {
			return is; // deleted row
		}

		for (const auto &col : self.tableInfo.columns) {
			is >> *self.cells.at(col.name);
		}

		return is;
	}

	std::ostream &operator<<(std::ostream &os, const DataRow &self) {
		const auto rowSize = self.getRowSize();
		writeSize(os, rowSize);

		for (const auto &col : self.tableInfo.columns) {
			os << *self.cells.at(col.name);
		}

		return os;
	}

	size_t DataRow::getRowSize() const {
		size_t size = 0;
		for (const auto &col : tableInfo.columns) {
			size += cells.at(col.name)->getRowSize();
		}
		return size;
	}

	DataCell &DataRow::atColumn(const ColumnInfo &column) const {
		return *cells.at(column.name);
	}
}
