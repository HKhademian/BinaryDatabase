#include "DataCell.h"
#include "DataRow.h"

namespace db {
	TypeFlag rowflag(bool isFree) {
		return isFree ? 0 : 1;
	}

	bool isRowFree(TypeFlag rowflag) {
		return rowflag & 1;
	}


	DataRow::DataRow(const TableInfo &info) : table(info) {
		for (const auto &col : table.columns) {
			cells.insert(std::pair<std::string, DataCell *>(col.name, new DataCell(col)));
		}
	}

	std::ostream &operator<<(std::ostream &os, DataRow &self) {
		if (isRowFree(self.flag)) { // free record
			if (self.sizeOnDisk) { // has size
				os.seekp(self.offset, os.beg);
				writeFlag(os, self.flag);
				writeSize(os, self.sizeOnDisk);
			}
			return os;
		}

		const auto rowSize = self.getRowSize();
		if (self.sizeOnDisk < rowSize) { // need new space
			// delete this
			writeFlag(os, rowflag(true));
			self.sizeOnDisk = 0;
		}
		if (self.sizeOnDisk <= 0) { // new allocation
			// TODO: find some other available rows
			os.seekp(std::ios::end);
			self.offset = os.tellp();
			self.sizeOnDisk = rowSize;
		}

		os.seekp(self.offset, os.beg);
		writeFlag(os, self.flag);
		writeSize(os, self.sizeOnDisk);

		if (!isRowFree(self.flag)) {
			for (const auto &col : self.table.columns) {
				os << *self.cells.at(col.name);
			}
		}

		return os;
	}

	std::istream &operator>>(std::istream &is, DataRow &self) {
		for (auto &cell : self.cells) {
			cell.second->clearValue();
		}

		const size_t dataSize = readSize(is);
		if (dataSize <= 0) {
			return is; // deleted row
		}

		for (const auto &col : self.table.columns) {
			is >> *self.cells.at(col.name);
		}

		return is;
	}

	size_t DataRow::getRowSize() const {
		size_t size = 0;
		for (const auto &col : table.columns) {
			size += cells.at(col.name)->getRowSize();
		}
		return size;
	}

	DataCell &DataRow::atColumn(const ColumnInfo &column) const {
		return *cells.at(column.name);
	}
}
