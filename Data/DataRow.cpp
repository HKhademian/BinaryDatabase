#include "../utils.h"
#include "DataCell.h"
#include "DataRow.h"

namespace db {
	DataRow::~DataRow() {
		// TODO: delete
		//for (auto *cell: cells) {
		//	delete cell;
		//}
		cells.clear();
	}

	DataRow::DataRow(const TableInfo &table)
		: RowInfo(table) {
		size_t cellOffset = 0;
		//add static size columns
		for (const auto &col : table.columns) {
			if (isDataTypeVar(col.type)) continue;
			cells.push_back(new DataCell(col, cellOffset));
			cellOffset += col.getRowSize();
		}
		//add variable size columns
		for (const auto &col : table.columns) {
			if (!isDataTypeVar(col.type)) continue;
			cells.push_back(new DataCell(col, cellOffset));
			cellOffset = -1;
		}
	}

	DataRow::DataRow(const RowInfo &rowInfo)
		: DataRow(rowInfo.table) {}

	size_t DataRow::getRowSize() const {
		size_t size = 0;
		for (const auto &col : table.columns) {
			size += atColumn(col)->getRowSize();
		}
		return size;
	}

	DataCell *DataRow::atColumn(const std::string &colName) const {
		for (auto *cell:cells) {
			if (cell->column.name == colName) {
				return cell;
			}
		}
		return nullptr;
	}

	DataCell *DataRow::atColumn(const ColumnInfo &column) const {
		return atColumn(column.name);
	}

	DataRow &DataRow::clear() {
		for (auto &cell:cells) {
			cell->clear();
		}
		return self;
	}

	std::ostream &operator<<(std::ostream &stream, const DataRow &row) {
		stream << row.table.name << "<";
		bool first = true;
		for (auto &column:row.table.columns) {
			auto &cell = *row.atColumn(column);
			if (!cell.hasValue()) continue; // skip non-value columns
			if (!first) stream << ",";
			first = false;
			stream << column.name << "=(" << cell << ")";
		}
		stream << ">";
		return stream;
	}

	std::istream &DataRow::readData(std::istream &stream, const std::vector<ColumnInfo> &columns) {
		readInfo(stream);
		calculateCellsOffset();
		if (offsetOnDisk == -1) return stream;
		if (isFree()) {
			for (auto &column:columns) {
				auto &cell = *atColumn(column);
				cell.clear();
			}
			return stream;
		}
		for (const auto &col : columns) {
			auto &cell = *atColumn(col);
			if (cell.offsetOnDisk == -1) continue; // skip ghost rows (even after each update we dont have its index, so leave it for safety)
			cell.readData(stream);
			if (cell.isTypeVar()) { // update rows offset after each variable row changed
				calculateCellsOffset();
			}
		}
		return stream;
	}

	std::ostream &DataRow::writeData(std::ostream &stream, const std::vector<ColumnInfo> &columns) {
		const auto dataSize = getRowSize();
		if (dataSize == 0 || sizeOnDisk == 0)
			throw std::logic_error("this must not happened!");
		if (sizeOnDisk == -1 || sizeOnDisk < getRowSize()) { // if new row OR required addition size
			if (sizeOnDisk != -1 && offsetOnDisk != -1 && !isFree()) {  // first free currently allocated space
				setFree(true);
				writeInfo(stream);
			}
			offsetOnDisk = -1; // need new row space
			sizeOnDisk = dataSize; // with dataSize space
		}
		writeInfo(stream);
		if (offsetOnDisk == -1) return stream; // if it's not on the disk
		if (isFree()) return seekpEnd(stream);
		calculateCellsOffset();
		for (auto &column:columns) {
			auto &cell = *atColumn(column);
			if (cell.offsetOnDisk == -1) continue; // skip ghost rows (even after update we dont have its index, so leave it for safety)
			cell.writeData(stream);
		}
		return seekpEnd(stream);
	}

	DataRow &DataRow::calculateCellsOffset() {
		size_t offset = offsetOnDisk + sizeof(TypeFlag) + sizeof(TypeSize);
// TODO: recalculate
//		//add static size columns
//		for (const auto &col : table.columns) {
//			if (isDataTypeVar(col.type)) continue;
//			cells.push_back(new DataCell(col, cellOffset));
//			cellOffset += col.getRowSize();
//		}
//		//add variable size columns
//		for (const auto &col : table.columns) {
//			if (!isDataTypeVar(col.type)) continue;
//			cells.push_back(new DataCell(col, cellOffset));
//			cellOffset = -1;
//		}
		for (auto *cell : cells) {
			cell->offsetOnDisk = offset;
			const auto size = cell->getSize();
			//offset = cell->isDataVar() ? -1 : offset + size;
			offset = offset + (cell->isDataVar() ? sizeof(TypeSize) : size); //never giveup mode
		}
		return self;
	}

}
