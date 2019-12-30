#include "../utils.h"
#include "DataTypeIO.h"
#include "ColumnInfo.h"
#include "TableInfo.h"
#include "RowInfo.h"

namespace db {
	RowInfo::RowInfo(const TableInfo &table)
		: table(table), sizeOnDisk(-1), offsetOnDisk(-1), flag(0), minSize(0) {
		minSize = 0;
		for (const auto &col: table.columns) {
			minSize += col.getRowSize();
		}
	}

	bool RowInfo::isFree() const {
		return flag == 0;
	}

	void RowInfo::setFree(bool isFree) {
		flag = isFree ? 0 : 1;
	}

	size_t RowInfo::unique() const {
		return offsetOnDisk;
	}

	std::istream &RowInfo::seekgEnd(std::istream &stream) {
		return stream.seekg(offsetOnDisk + sizeof(TypeFlag) + sizeof(TypeSize) + sizeOnDisk, std::istream::beg);
	}

	std::ostream &RowInfo::seekpEnd(std::ostream &stream) {
		return stream.seekp(offsetOnDisk + sizeof(TypeFlag) + sizeof(TypeSize) + sizeOnDisk, std::istream::beg);
	}

	std::istream &RowInfo::seekg(std::istream &stream) {
		return stream.seekg(offsetOnDisk, std::istream::beg);
	}

	std::ostream &RowInfo::seekp(std::ostream &stream) {
		return stream.seekp(offsetOnDisk, std::ostream::beg);
	}

	std::istream &RowInfo::readInfo(std::istream &stream) {
		if (offsetOnDisk != -1) { // already on disk
			seekg(stream);
		}
		offsetOnDisk = stream.tellg();
		flag = readFlag(stream);
		sizeOnDisk = readSize(stream);
		return seekgEnd(stream);
	}

	std::ostream &RowInfo::writeInfo(std::ostream &stream) {
		bool isNewRow = false;
		if (sizeOnDisk == -1) {
			// return stream; // new row without any data
			sizeOnDisk = DEFAULT_ROW_SIZE; // default row size
		}
		if (offsetOnDisk == -1) { // new row
			// TODO: may find free rows instead
			stream.seekp(0, std::ostream::end);
			offsetOnDisk = stream.tellp();
			isNewRow = true;
		} else {
			seekp(stream);
		}
		writeFlag(stream, flag);
		writeSize(stream, sizeOnDisk);
		if (isNewRow || isFree()) { // write zero place holders
			writeBin(stream, nullptr, sizeOnDisk);
		}
		return seekp(stream);
	}

	RowInfo &RowInfo::readInfo() {
		auto stream = table.openDataInputStream();
		readInfo(stream);
		stream.close();
		return self;
	}

	RowInfo &RowInfo::writeInfo() {
		std::ofstream stream = table.openDataOutputStream();
		writeInfo(stream);
		stream.close();
		return self;
	}

	bool RowInfo::operator==(const RowInfo &rhs) const {
		return &rhs == this || (
			self.table == rhs.table &&
			self.offsetOnDisk == rhs.offsetOnDisk &&
			self.sizeOnDisk == rhs.offsetOnDisk
		);
	}

	bool RowInfo::operator<(const RowInfo &rhs) const {
		return &rhs != this || self.offsetOnDisk < rhs.offsetOnDisk || self.table < rhs.table;
	}

}
