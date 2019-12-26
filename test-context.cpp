#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Database.h"

using namespace std;
using namespace db;
using namespace db::ctx;

bool shell(Context &context, const string &cmd) {
	static size_t line = 0;
	try {
		cout << ++line << ") " << cmd << endl << flush;
		const auto result = context.exec(cmd);

		if (result.type == Result::DONE) {
			cout << line << ": " << "DONE!" << endl << flush;
		} else if (result.type == Result::ERR) {
			cerr << line << ": " << "ERR!" << endl << flush;
		}

		return true;
	} catch (exception &err) {
		cerr << line << ": " << err.what() << endl << flush;
		return false;
	}
}

void onDatabaseOpen(Context &context, int version) {
	shell(context, "CreateTable(\"test1\", col1    : int   , 'col2' :    byte[15], `col3`:text)");
	shell(context, "CreateTable(mytbl2, col1:int, c2:real[2], name:char[1])");
	shell(context, "  deleteTable   (test1)");
	shell(context, "CreateTable(\"test1\", col5    : int)");

	shell(context, "select(\"test1\", col5    : int)");

	shell(context, "insert(\"test1\", col5    = int)");
	shell(context, "insert(test1, {col5    : int})");
	shell(context, "insert(test3, {col5    = int})");
	shell(context, "insert(mytbl2, {col5    = int})");
	shell(context, "insert(mytbl2, {col1    = 10})");
}

void mainContext() {
	Context context;
	context.open("db1", 1, onDatabaseOpen, nullptr, nullptr);
	context.close();
}




