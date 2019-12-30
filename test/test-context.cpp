#include<cstdarg>
#include <iostream>
#include <cstdlib>
#include "../Database.h"

using namespace std;
using namespace db;
using namespace db::ctx;

bool shell(Context &context, const char *cmd, ...) {
	static size_t line = 0;
	cout << flush;
	cerr << flush;

	try {
		cout << ++line << ") " << cmd << endl;

		va_list vargs;
		va_start(vargs, cmd);
		auto &result = context.execArgs(cmd, vargs);
		va_end(vargs);

		if (!result.hasError()) {
			cout << line << ": " << "DONE!";
			const auto &rows = result.getRows();
			cout << " (" << rows.size() << " Result Rows)" << endl;
			for (auto &row : rows) {
				cout << "\t" << row << endl;
			}
			return true;
		} else {
			cerr << line << ": " << "ERR! (" << result.res().message << ")" << endl;
			return false;
		}

	} catch (exception &err) {
		cerr << line << ": unhandled error: " << err.what() << endl;
	}
	return false;
}

void onDatabaseOpen(Context &context, int version) {
	cout << "onDatabaseOpen(version:" << version << ")" << endl;
////	context.exec("CreateTable(${s1}, ${ s2  } : int, 'col2': byte[${i3}], ${s4}:text) [ { 2, ${r5} }, '', $ { 2}  '', ${i4} ]", "myTable", "col2", 15, "firstname");
////	context.exec("${text} ${text} ${int} ${text}", "myTable", "col2", 15, "firstname");
////	context.exec("${2:text} ${1:text} ${3:int} ${text}", "myTable", "col2", 15, "firstname");
//	shell(context, "${text} ${text} ${int} ${text}", "myTable", "col2", 15, "firstname");
//
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
		shell(context, "Insert (students, {id=1}, {id=2}, {id=3})");

		shell(context, "Insert (students, {id=${int}"     ", name=`Saeed Khademian`"  ", grade=18.0"     ", year=94})", 97);

		shell(context, "Insert (students, {id=${ 2 :int }"", name=${ text }"          ", grade=-1.1"     ", year=95})", "Hossain", 1374);

		loopIn(i, 5, 10) shell(context, "Insert     (students, {id=${int},name=${text},grade=${real},year=${byte}})", i, "TestRecord", 17.23, i * 2);
	} else {
		shell(context, "Select(students,[year])");
		shell(context, "Select(students,eq(year,255))");
		shell(context, "Select(students,eq(year,94))");

		shell(context, "Select(students,neq(id,1374))");
		shell(context, "Select(students,eq(id,1374))");

		shell(context, "Select(students,neq(year,94))");
		shell(context, "Select(students,eq(year,94))");

		shell(context, "Select(students,and( neq(year,94) , neq(id,1374) ))");
		shell(context, "Select(students,and( eq(year,94) , eq(id,1374) ))");

		shell(context, "Select(students,or( neq(year,94) , neq(id,1374) ))");
		shell(context, "Select(students,or( eq(year,94) , eq(id,1374) ))");

		shell(context, "Select(students,not(and( neq(year,94) , neq(id,1374) )))");
		shell(context, "Select(students,not(or( neq(year,94) , neq(id,1374) )))");

		shell(context, "Update(students,{year=99},eq(id, 97))");
		shell(context, "Update(students,{name=${text},grade=19.9},eq(id,${int}))", "Hossain Khademian (New)", 1374);

		shell(context, "Select     (students, eq(id, 2))");
		shell(context, "Select     (students, ge(id, 2))");
		shell(context, "Select     (students, le(id, 2))");
		shell(context, "Select     (students, not( eq(id, 2)) )");
		shell(context, "Select     (students, nge(id, 5))");
	}
}

void onDatabaseCreate(Context &context, int version) {
	cout << "onDatabaseCreate(version:" << version << ")" << endl;
}

void onDatabaseMigrate(Context &context, int version, int oldVersion) {
	cout << "onDatabaseMigrate(version:" << version << ")" << endl;
}

void mainContext() {
	Context context;
	context.open("myTestDatabase", 1, &onDatabaseOpen, &onDatabaseCreate, &onDatabaseMigrate);
	context.close();
}




