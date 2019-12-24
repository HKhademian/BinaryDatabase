#include<cstdarg>
#include<string>
#include <cstring>
#include <iostream>
#include <fstream>
#include "DataType.h"
#include "ColumnInfo.h"
#include "TableInfo.h"
#include "DatabaseInfo.h"
#include "Database.h"

using namespace std;

namespace Database {
	int execDB();

	class Context { // current context
		string &db_name;

	};


	int exec(const string *command, ...) {
		va_list args;
		va_start(args, command);

		int result = 0;
		//if (command.compare("DB") == 0) {
		//	result = execDB(va_arg(args, char));
		//}

		va_end(args);

		return result;
	}

	int execLoadDatabase(const string *name) {
//		string path;
//		path.append("./database/");
//		path.append(name);
//		path.append(".db");
		//const string path = "./database/" + name + ".db";
		//ifstream file;
		//file.open(path.c_str(), ios::in | ios::app | ios::binary);
		//file.getline()
		//char *bin = new char[file.]
		//file.read()
		//osf.close();
		return 0;
	}

	int execDB() {
		return 0;

	}
}
