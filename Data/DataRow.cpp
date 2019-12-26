#include "DataCell.h"
#include "DataRow.h"

namespace db {
	TypeFlag rowFlag(bool isFree) {
		return isFree ? 0 : 1;
	}

	bool isRowFree(TypeFlag row) {
		return (row & 1) == 1;
	}

	DataRow::DataRow(const TableInfo &info) : table(info) {
		size_t cellOffset = 0;
		//add static size columns
		for (const auto &col : table.columns) {
			const auto size = col.getRowSize();
			if (size <= 0 || size == -1) continue;
			cells.insert(std::pair<std::string, DataCell *>(
				col.name,
				new DataCell(col, cellOffset)
			));
			cellOffset += size;
		}
		//add variable size columns
		for (const auto &col : table.columns) {
			const auto size = col.getRowSize();
			if (size > 0 && size != -1) continue;
			cells.insert(std::pair<std::string, DataCell *>(
				col.name,
				new DataCell(col, cellOffset)
			));
			cellOffset = -1; // uint max means veriable
		}
	}

	std::ostream &operator<<(std::ostream &os, DataRow &self) {
		if (isRowFree(self.flag)) { // free record
			if (self.sizeOnDisk) { // has size
				os.seekp(self.offset, std::ostream::beg);
				writeFlag(os, self.flag);
				writeSize(os, self.sizeOnDisk);
			}
			return os;
		}

		const auto rowSize = self.getRowSize();
		if (self.sizeOnDisk < rowSize) { // need new space
			// delete this
			writeFlag(os, rowFlag(true));
			self.sizeOnDisk = 0;
		}
		if (self.sizeOnDisk <= 0) { // new allocation
			// TODO: find some other available rows
			os.seekp(std::ios::end);
			self.offset = os.tellp();
			self.sizeOnDisk = rowSize;
		}

		os.seekp(self.offset, std::ostream::beg);
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
		self.readInfo(is);

		if (isRowFree(self.flag)) {
			return is;
		}

		for (const auto &col : self.table.columns) {
			is >> *self.cells.at(col.name);
		}

		return is;
	}

	std::istream &DataRow::readInfo(std::istream &is) {
		offset = is.tellg();
		flag = readFlag(is);
		sizeOnDisk = readSize(is);


		size_t cellOffset = 0;
		for (auto &cellPair : cells) { // first static types
			auto &cell = *cellPair.second;
			if (cell.size == 0 || cell.size == -1) continue;
			cell.clearValue();
			cell.offsetOnRow = cellOffset;
			auto diskSize = cell.size;
			cellOffset += diskSize;
			is.seekg(diskSize, std::istream::cur);
		}
		for (auto &cellPair : cells) { // then variant types
			auto &cell = *cellPair.second;
			if (cell.size != 0 && cell.size != -1) continue;
			cell.clearValue();
			cell.offsetOnRow = cellOffset;
			auto diskSize = readSize(is);
			cellOffset += diskSize + sizeof(TypeSize);
			is.seekg(diskSize, std::istream::cur);
		}

		return is;
	}

	std::istream &DataRow::readData(std::istream &is, const std::vector<ColumnInfo> &columns) {
		for (auto &column:columns) {
			auto &cell = atColumn(column);
			is.seekg(offset + cell.offsetOnRow);
			is >> cell;
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
