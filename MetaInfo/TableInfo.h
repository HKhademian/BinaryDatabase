#ifndef DATABASE_TABLE_INFO_H
#define DATABASE_TABLE_INFO_H

#include <vector>
#include <fstream>
#include "../utils.h"

namespace db {
	struct DatabaseInfo;
	struct ColumnInfo;

	struct TableInfo {
		const DatabaseInfo &database;
		std::string name = "";
		std::vector<ColumnInfo> columns;

		explicit TableInfo(const DatabaseInfo &database, std::string nam = "") :
			database(database), name(std::move(nam)) {}

		bool operator==(const TableInfo &rhs) const;

		bool operator<(const TableInfo &rhs) const;

		std::ostream &writeInfo(std::ostream &stream) const;

		std::istream &readInfo(std::istream &stream);


		size_t getRowSize() const;

		size_t getDataOffset(int index) const;

		int columnPos(const std::string &columnName) const;

		const ColumnInfo *column(const std::string &columnName) const;

		std::string getDataPath() const;

		std::fstream &openDataStream(std::fstream &stream) const;

		std::fstream openDataStream() const;

		std::ifstream &openDataInputStream(std::ifstream &stream) const;

		std::ifstream openDataInputStream() const;

		std::ofstream &openDataOutputStream(std::ofstream &stream) const;

		std::ofstream openDataOutputStream() const;
	};
}
#endif
