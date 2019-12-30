#ifndef DATABASE_DATABASE_INFO_H
#define DATABASE_DATABASE_INFO_H

#include <vector>
#include <fstream>

namespace db {
	struct TableInfo;

	struct DatabaseInfo {
		TypeText name = "";
		TypeSize version = 1;
		std::vector<TableInfo> tables;

		DatabaseInfo() = default;

		explicit DatabaseInfo(std::string nam) :
			name(std::move(nam)) {}

		std::ostream &writeInfo(std::ostream &stream) const;

		const DatabaseInfo &writeInfo() const;

		std::istream &readInfo(std::istream &stream);

		DatabaseInfo &readInfo();


		int tablePos(const std::string &tableName) const;

		const TableInfo *table(const std::string &tableName) const;


		std::string getInfoPath() const;

		std::fstream &openInfoStream(std::fstream &stream) const;

		std::fstream openInfoStream() const;

		std::ifstream &openInfoInputStream(std::ifstream &stream) const;

		std::ifstream openInfoInputStream() const;

		std::ofstream &openInfoOutputStream(std::ofstream &stream) const;

		std::ofstream openInfoOutputStream() const;

	};

}
#endif
