#ifndef DATABASE_H
#define DATABASE_H

#include<string>
#include "DataType.h"
#include "ColumnInfo.h"
#include "TableInfo.h"
#include "DatabaseInfo.h"
#include "DataCell.h"
#include "DataRow.h"
#include "utils.h"


namespace Database {

	int exec(const std::string *command, ...);

	int execLoadDatabase(const std::string *name);
}

#endif