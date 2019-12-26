#ifndef DATABASE_DATA_TABLE_H
#define DATABASE_DATA_TABLE_H

#include "../MetaInfo/TableInfo.h"
#include "DataRow.h"

namespace db {
	std::vector<DataRow> loadRows(const TableInfo &table);

	void insert(const DataRow &row) ;
}

#endif
