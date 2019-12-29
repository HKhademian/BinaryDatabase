#include "DataCell.h"
#include "DataRow.h"
#include "../utils.h"

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
		if (sizeOnDisk == -1) { // new row
			offsetOnDisk = -1;
			sizeOnDisk = dataSize;
		} else if (sizeOnDisk < getRowSize()) { // required addition size
			const auto free = isFree();
			setFree(true);
			writeInfo(stream);
			offsetOnDisk = -1;
			sizeOnDisk = dataSize;
			setFree(free);
		}
		writeInfo(stream);
		calculateCellsOffset();
		if (offsetOnDisk == -1) return stream;
		if (isFree()) { // free record
			stream.seekp(offsetOnDisk + sizeof(TypeFlag) + sizeof(TypeSize) + sizeOnDisk, std::ostream::beg);
			// TODO: maybe zero old data
			// TODO: choose free records write variable size data len or not
			return stream;
		}
		for (auto &column:columns) {
			auto &cell = *atColumn(column);
			if (cell.offsetOnDisk == -1) continue; // skip ghost rows (even after update we dont have its index, so leave it for safety)
			cell.writeData(stream);
		}
		return stream;
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
