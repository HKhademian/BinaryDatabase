#include <iostream>
#include "../utils.h"
#include "DataTypeIO.h"
#include "ColumnInfo.h"
#include "DatabaseInfo.h"
#include "TableInfo.h"

namespace db {
	std::string TableInfo::getDataPath() const {
		return "./tbl-" + self.database.name + "-" + self.name + ".dat";
	}

	std::ostream &TableInfo::writeInfo(std::ostream &stream) const {
		writeText(stream, self.name);
		const auto count = self.columns.size();
		writeSize(stream, count);
		loop (i, count) {
			self.columns[i].writeInfo(stream);
		}
		return stream;
	}

	std::istream &TableInfo::readInfo(std::istream &stream) {
		self.columns.clear();
		readText(stream, self.name);
		const size_t count = readSize(stream);
		loop (i, count) {
			auto col = ColumnInfo(); // auto el = new ColumnInfo();
			col.readInfo(stream);
			self.columns.push_back(col);
		}
		return stream;
	}

	size_t TableInfo::getRowSize() const {
		const auto count = columns.size();
		size_t sum = 0;
		loop(i, count) {
			sum += columns[i].getRowSize();
		}
		return sum;
	}

	size_t TableInfo::getDataOffset(int index) const {
		size_t offset = 0;
		loop(i, index) {
			const auto &col = columns[i];
			offset += col.getRowSize();
		}
		return offset;
	}

	int TableInfo::columnPos(const std::string &columnName) const {
		loop(i, columns.size()) {
			if (strcaseequal(columns[i].name, columnName)) {
				return i;
			}
		}
		return -1;
	}

	const ColumnInfo *TableInfo::column(const std::string &columnName) const {
		for (const auto &column : columns) {
			if (strcaseequal(column.name, columnName)) {
				return &column;
			}
		}
		return nullptr;
	}

	std::fstream &TableInfo::openDataStream(std::fstream &stream) const {
		const auto &path = getDataPath();
		stream.open(path, std::ios::out | std::ios::binary); // create if not exists
		stream.close();
		stream.open(path, std::ios::in | std::ios::out | std::ios::binary); // open for rw
		stream.seekg(0, std::ifstream::beg);
		stream.seekp(0, std::ifstream::beg);
		return stream;
	}

	std::fstream TableInfo::openDataStream() const {
		std::fstream stream;
		openDataStream(stream);
		return stream;
	}

	std::ifstream &TableInfo::openDataInputStream(std::ifstream &stream) const {
		const auto &path = getDataPath();
		stream.open(path, std::ios::in | std::ios::binary); // open if exists
		if (!stream) {
			stream.open(path, std::ios::out | std::ios::binary); // create if not
			stream.close();
			stream.open(path, std::ios::in | std::ios::binary);
		}
		stream.seekg(0, std::ifstream::beg);
		return stream;
	}

	std::ifstream TableInfo::openDataInputStream() const {
		std::ifstream stream;
		openDataInputStream(stream);
		return stream;
	}

	std::ofstream &TableInfo::openDataOutputStream(std::ofstream &stream) const {
		const auto &path = getDataPath();
		stream.open(path, std::ios::in | std::ios::out | std::ios::binary); // open if exists
		if (!stream) {
			stream.open(path, std::ios::out | std::ios::binary); // create if not
		}
		stream.seekp(0, std::ifstream::beg);
		return stream;
	}

	std::ofstream TableInfo::openDataOutputStream() const {
		std::ofstream stream;
		openDataOutputStream(stream);
		return stream;
	}

	bool TableInfo::operator==(const TableInfo &rhs) const {
		return &rhs == this || self.name == rhs.name;
	}

	bool TableInfo::operator<(const TableInfo &rhs) const {
		return &rhs != this && self.name < rhs.name;
	}

}
