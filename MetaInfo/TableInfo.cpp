#include <iostream>
#include "DataTypeIO.h"
#include "ColumnInfo.h"
#include "TableInfo.h"
#include "../utils.h"

namespace db {
	std::string TableInfo::getDataPath() const {
		return "./tbl-" + name + ".dat";
	}


	std::ostream &operator<<(std::ostream &os, const TableInfo &data) {
		writeText(os, data.name);

		const auto count = data.columns.size();
		writeSize(os, count);

		loop (i, count) {
			os << data.columns[i];
		}

		return os;
	}

	std::istream &operator>>(std::istream &is, TableInfo &data) {
		data.columns.clear();

		readText(is, data.name);

		const size_t count = readSize(is);
		loop (i, count) {
			auto el = new ColumnInfo();
			is >> *el;
			data.columns.push_back(*el);
		}

		return is;
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
		stream.open(getDataPath(), std::ios::out | std::ios::binary); // create if not exists
		stream.close();
		stream.open(getDataPath(), std::ios::in | std::ios::out | std::ios::binary); // open for rw
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
		stream.open(getDataPath(), std::ios::in | std::ios::binary); // open if exists
		if (!stream) {
			stream.open(getDataPath(), std::ios::out | std::ios::binary); // create if not
			stream.close();
			stream.open(getDataPath(), std::ios::in | std::ios::binary);
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
		stream.open(getDataPath(), std::ios::in | std::ios::out | std::ios::binary); // open if exists
		if (!stream) {
			stream.open(getDataPath(), std::ios::out | std::ios::binary); // create if not
		}
		stream.seekp(0, std::ifstream::beg);
		return stream;
	}

	std::ofstream TableInfo::openDataOutputStream() const {
		std::ofstream stream;
		openDataOutputStream(stream);
		return stream;
	}

}
