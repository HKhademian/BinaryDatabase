#include <iostream>
#include <cstdlib>
#include <fstream>
#include "../Database.h"

using namespace std;
using namespace db;
using namespace db::ctx;

bool shell(Context &context, const string &cmd) {
	static size_t line = 0;
	cout << flush;
	cerr << flush;
	try {
		cout << ++line << ") " << cmd << endl;
		auto &result = context.exec(cmd);

		if (!result.hasError()) {
			cout << line << ": " << "DONE!";
			const auto &rows = result.rows();
			if (!rows.empty()) {
				cout << " (" << rows.size() << ")";
			}
			cout << endl;
			return true;
		} else {
			cerr << line << ": " << "ERR!" << endl;
			return false;
		}

	} catch (exception &err) {
		cerr << line << ": " << err.what() << endl;
	}
	return false;
}

void onDatabaseOpen(Context &context, int version) {
//	shell(context, "CreateTable(\"test1\", col1    : int   , 'col2' :    byte[15], `col3`:text)");
//	shell(context, "CreateTable(mytbl2, col1:int, c2:real[2], name:char[1])");
//	shell(context, "  deleteTable   (test1)");
//	shell(context, "CreateTable(\"test1\", col5    : int)");
//
//	shell(context, "select(\"test1\", col5    : int)");
//	shell(context, "eq(col5,2)");
//	shell(context, "select(  test1,   eq(col5,2)    )");
//
//	shell(context, "insert(\"test1\", col5    = int)");
//	shell(context, "insert(test1, {col5    : int})");
//	shell(context, "insert(test3, {col5    = int})");
//	shell(context, "insert(mytbl2, {col5    = int})");
//	shell(context, "insert(mytbl2, {col1    = 10})");

	if (shell(context, "CREATETABLE(students, id:int, name:text, grade:real, year:byte)")) {
		shell(context, "Insert     (students, {id=1, name='Hossain Khademian', grade=18.9,  year=97})");
		shell(context, "Insert     (students, {id=2, name=`Saeed Khademian`,   grade=18.0,  year=94})");
		shell(context, "Insert     (students, {id=3, name=\"Unknown\",         grade=-1.1,  year=95})");
		shell(context, "Insert     (students, {id=3, name=Unknown,             grade=-1.1,  year=95})");
	}
	shell(context, "Select     (students, eq(id, 2))");
	shell(context, "Select     (students, ge(id, 2))");
	shell(context, "Select     (students, le(id, 2))");
	shell(context, "Select     (students, not( eq(id, 2)) )");
	shell(context, "Select     (students, nge(id, 5))");

}

void mainContext() {
	Context context;
	context.open("db1", 1, &onDatabaseOpen, nullptr, nullptr);
	context.close();
}




