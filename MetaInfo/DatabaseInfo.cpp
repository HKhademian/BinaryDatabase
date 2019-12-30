#include <iostream>
#include "../utils.h"
#include "ColumnInfo.h"
#include "TableInfo.h"
#include "DataTypeIO.h"
#include "DatabaseInfo.h"

namespace db {
	std::string DatabaseInfo::getInfoPath() const {
		return "./db-" + self.name + ".dat";
	}

	std::ostream &DatabaseInfo::writeInfo(std::ostream &stream) const {
		writeSize(stream, self.version);
		writeText(stream, self.name);
		const auto tableCount = self.tables.size();
		writeSize(stream, tableCount);
		loop (i, tableCount) {
			self.tables[i].writeInfo(stream);
		}
		return stream;
	}

	const DatabaseInfo &DatabaseInfo::writeInfo() const {
		auto stream = openInfoOutputStream();
		writeInfo(stream);
		stream.close();
		return self;
	}

	std::istream &DatabaseInfo::readInfo(std::istream &stream) {
		self.tables.clear();
		readSize(stream, self.version);
		readText(stream, self.name);
		const TypeSize tableCount = readSize(stream);
		loop (i, tableCount) {
			auto table = *new TableInfo;
			table.readInfo(stream);
			self.tables.push_back(table);
		}
		return stream;
	}

	DatabaseInfo &DatabaseInfo::readInfo() {
		auto stream = openInfoInputStream();
		readInfo(stream);
		stream.close();
		return self;
	}


	int DatabaseInfo::tablePos(const std::string &tableName) const {
		loop(i, tables.size()) {
			if (strcaseequal(tables[i].name, tableName)) {
				return i;
			}
		}
		return -1;
	}

	const TableInfo *DatabaseInfo::table(const std::string &tableName) const {
		for (const auto &table : tables) {
			if (strcaseequal(table.name, tableName)) {
				return &table;
			}
		}
		return nullptr;
	}


	std::fstream &DatabaseInfo::openInfoStream(std::fstream &stream) const {
		const auto &path = getInfoPath();
		stream.open(path, std::ios::out | std::ios::binary); // create if not exists
		stream.close();
		stream.open(path, std::ios::in | std::ios::out | std::ios::binary); // open for rw
		stream.seekg(0, std::ifstream::beg);
		stream.seekp(0, std::ifstream::beg);
		return stream;
	}

	std::fstream DatabaseInfo::openInfoStream() const {
		std::fstream stream;
		openInfoStream(stream);
		return stream;
	}

	std::ifstream &DatabaseInfo::openInfoInputStream(std::ifstream &stream) const {
		const auto &path = getInfoPath();
		stream.open(path, std::ios::in | std::ios::binary); // open if exists
		if (!stream) {
			stream.open(path, std::ios::out | std::ios::binary); // create if not
			stream.close();
			stream.open(path, std::ios::in | std::ios::binary);
		}
		stream.seekg(0, std::ifstream::beg);
		return stream;
	}

	std::ifstream DatabaseInfo::openInfoInputStream() const {
		std::ifstream stream;
		openInfoInputStream(stream);
		return stream;
	}

	std::ofstream &DatabaseInfo::openInfoOutputStream(std::ofstream &stream) const {
		const auto &path = getInfoPath();
		stream.open(path, std::ios::in | std::ios::out | std::ios::binary); // open if exists
		if (!stream) {
			stream.open(path, std::ios::out | std::ios::binary); // create if not
		}
		stream.seekp(0, std::ifstream::beg);
		return stream;
	}

	std::ofstream DatabaseInfo::openInfoOutputStream() const {
		std::ofstream stream;
		openInfoOutputStream(stream);
		return stream;
	}

}
