#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include "utils.h"
#include "Database.h"
#include "MetaInfo/DataType.h"
#include "MetaInfo/ColumnInfo.h"
#include "MetaInfo/TableInfo.h"
#include "Data/DataRow.h"

using namespace std;
using namespace db;


int main() {
	void mainContext();
	mainContext();
	return 0;

	//void mainStudents();
	//mainStudents();
	//return 0;
}

//const auto file = "test.bin";
//void test1() {
//	{
//		ofstream os;
//		os.open(file, ios::out | ios::app | ios::binary);
//
//		ColumnInfo info = ColumnInfo(
//				dataType(TYPE_INT, 1),
//				"Column1"
//		);
//		cout << info << endl;
//
//		os << info;
//		os.close();
//	}
//	cout << endl;
//	{
//		ifstream is;
//		is.open(file, ios::in | ios::app | ios::binary);
//		ColumnInfo info;
//		is >> info;
//		is.close();
//		cout << info << endl;
//	}
//}
//
//
//void test2() {
//	cout << endl;
//
//	{
//		ofstream os;
//		os.open(file, ios::out | ios::binary);
//
//		TableInfo table;
//		table.name = "Table1";
//		loopn(i, 10) {
//			ColumnInfo col;
//			col.type = dataType(i % 2 ? SIZE_INT : SIZE_REAL);
//			col.name = table.name + "$Column#" + std::to_string(i);
//			table.columns.push_back(col);
//		}
//		cout << table << endl;
//
//		os << table;
//		os.close();
//	}
//	cout << endl;
//
//	{
//		ifstream is;
//		is.open(file, ios::in | ios::app | ios::binary);
//		TableInfo info;
//		is >> info;
//		is.close();
//		cout << info << endl;
//	}
//}
//
//
//void testDataCell() {
//	ofstream os;
//	os.open(file, ios::out | ios::binary);
//
//	TableInfo tableStudents("students");
//
//	ColumnInfo colName(DATA_TYPE_TEXT, "name");
//	//os << "ColName: " << colName << endl;
//
//	tableStudents.columns.push_back(colName);
//	//os << "TblStudents: " << tableStudents << endl;
//
//	DataRow row1(tableStudents);
//	{
//		DataCell cellPK1(PK);
//		cellPK1.setInt(10);
//		row1.cells.push_back(cellPK1);
//		//os << "Cell1PK: " << cellPK1 << endl;
//
//		DataCell cellName1(colName);
//		cellName1.setText("Hossain Khademian");
//		row1.cells.push_back(cellName1);
//		//os << "Cell1Name: " << cellName1 << endl;
//	}
//	//os << "Row1: ";
//	os << row1;
//	//os << endl;
//
//	os.close();
//}
