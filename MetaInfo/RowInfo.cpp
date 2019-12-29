#include <fstream>
#include "RowInfo.h"
#include "DataTypeIO.h"
#include "../utils.h"

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

	std::istream &RowInfo::readInfo(std::istream &is) {
		if (offsetOnDisk != -1) { // already on disk
			is.seekg(offsetOnDisk, std::istream::beg);
		}
		offsetOnDisk = is.tellg();
		flag = readFlag(is);
		sizeOnDisk = readSize(is);
		//is.seekg(sizeOnDisk, std::istream::cur);
		is.seekg(offsetOnDisk + sizeof(TypeFlag) + sizeof(TypeSize) + sizeOnDisk, std::istream::beg);
		return is;
	}

	std::ostream &RowInfo::writeInfo(std::ostream &stream) {
		if (sizeOnDisk == -1) {
			// return stream; // new row without any data
			sizeOnDisk = 128; // default row size
		}
		if (offsetOnDisk == -1) { // new row
			stream.seekp(0, std::ostream::end);
			offsetOnDisk = stream.tellp();
		} else {
			stream.seekp(offsetOnDisk, std::ostream::beg);
		}
		writeFlag(stream, flag);
		writeSize(stream, sizeOnDisk);
		return stream;
	}

	RowInfo &RowInfo::readInfo() {
		std::ifstream is;
		table.openDataInputStream(is);
		readInfo(is);
		is.close();
		return self;
	}

	RowInfo &RowInfo::writeInfo() {
		std::ofstream os;
		table.openDataOutputStream(os);
		writeInfo(os);
		os.close();
		return self;
	}

}
