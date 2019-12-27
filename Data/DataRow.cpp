#include "DataCell.h"
#include "DataRow.h"

namespace db {
	void rowFlag(TypeFlag &flag, bool isFree) {
		flag &= ~(1 << 0);//clear free flag
		flag |= (isFree ? 0 : 1) << 0; // set free flag
	}

	TypeFlag rowFlag(bool isFree) {
		TypeFlag flag = 0;
		rowFlag(flag, isFree);
		return flag;
	}

	bool isRowFree(TypeFlag flag) {
		return (flag & ((TypeFlag) 1 << 0)) == 0;
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
			if (self.sizeOnDisk > 0 && self.sizeOnDisk != -1) { // has size
				if (self.offset == -1) {
					//TODO: maybe zero old data
					// TODO: choose free records write variable size data len or not
					os.seekp(std::ostream::end);
					self.offset = os.tellp();
				} else {
					os.seekp(self.offset, std::ostream::beg);
				}
				writeFlag(os, self.flag);
				writeSize(os, self.sizeOnDisk);
			}
			return os;
		}

		const auto rowSize = self.getRowSize();
		if (self.sizeOnDisk == -1 || self.sizeOnDisk < rowSize) { // need new space
			if (self.offset != -1) {// delete this record
				writeFlag(os, rowFlag(true));
			}
			self.sizeOnDisk = 0;
		}
		if (self.sizeOnDisk == 0 || self.sizeOnDisk == -1) { // new allocation
			// TODO: find some other available rows
			os.seekp(0, std::ios::end);
			self.offset = os.tellp();
			self.sizeOnDisk = rowSize;
		}

		os.seekp(self.offset, std::ostream::beg);
		writeFlag(os, self.flag);
		writeSize(os, self.sizeOnDisk);

		for (const auto &col : self.table.columns) { // first fixed types
			auto &cell = *self.cells.at(col.name);
			if (cell.size == 0 || cell.size == -1) continue;
			os << *self.cells.at(col.name);
		}

		for (const auto &col : self.table.columns) { // then variant types
			auto &cell = *self.cells.at(col.name);
			if (cell.size != 0 && cell.size != -1) continue;
			os << *self.cells.at(col.name);
		}

		return os;
	}

	std::istream &operator>>(std::istream &is, DataRow &row) {
		row.readInfo(is);

		if (isRowFree(row.flag)) {
			return is;
		}

		for (const auto &col : row.table.columns) { // first fixed types
			auto &cell = *row.cells.at(col.name);
			if (cell.size == 0 || cell.size == -1) continue;
			is >> *row.cells.at(col.name);
		}

		for (const auto &col : row.table.columns) { // then variant types
			auto &cell = *row.cells.at(col.name);
			if (cell.size != 0 && cell.size != -1) continue;
			is >> *row.cells.at(col.name);
		}

		return is;
	}

	std::istream &DataRow::readInfo(std::istream &is) {
		offset = is.tellg();
		flag = readFlag(is);
		sizeOnDisk = readSize(is);

		size_t cellOffset = 0;
		for (const auto &col : table.columns) { // first fixed types
			auto &cell = *cells.at(col.name);
			if (cell.size == 0 || cell.size == -1) continue;
			cell.clear();
			cell.offsetOnRow = cellOffset;
			auto diskSize = cell.size;
			cellOffset += diskSize;
			is.seekg(diskSize, std::istream::cur);
		}
		for (const auto &col : table.columns) { // then variant types
			auto &cell = *cells.at(col.name);
			if (cell.size != 0 && cell.size != -1) continue;
			cell.clear();
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
			is.seekg(offset + cell.offsetOnRow + sizeof(TypeFlag) + sizeof(TypeSize), std::istream::beg);
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

	bool DataRow::isFree() const {
		return isRowFree(flag);
	}

	void DataRow::setFree(bool isFree) {
		rowFlag(this->flag, isFree);
	}

	size_t DataRow::unique() const {
		return offset;
	}

}
