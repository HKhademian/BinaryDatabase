#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Database.h"

using namespace std;
using namespace db;

struct MenuItem {
	string title;

	void (*action)();
};

int getChoose();

void showMenu();

void menuList();

void menuAdd();

void menuFind();

TableInfo tableStudents("students");
ColumnInfo colNum(TYPE_INT, "num");
ColumnInfo colName(TYPE_TEXT, "name");
ColumnInfo colYear(TYPE_INT, "year");
ColumnInfo colGrade(TYPE_REAL, "grade");
const auto file = "students.bin";

const int menuItemCount = 4;

const MenuItem menuItems[menuItemCount] = {
		{"List Students",   menuList},
		{"Add new Student", menuAdd},
		{"Find Student",    menuFind},
		{"Exit",            nullptr},
};

void mainStudents() {
	tableStudents.columns.push_back(colNum);
	tableStudents.columns.push_back(colName);
	tableStudents.columns.push_back(colYear);
	tableStudents.columns.push_back(colGrade);

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


	while (true) {
		const auto choose = getChoose();
		const auto action = menuItems[choose].action;
		if (action == nullptr) {
			cout << "Good Bye!" << endl;
			break;
		}
		(*action)();
		system("pause");
	}
}

void showMenu() {
	system("cls");
	int pos = 0;
	for (const auto &menuItem : menuItems) {
		cout << pos++ << ": " << menuItem.title << endl;
	}
}

int getChoose() {
	int choose;
	do {
		showMenu();
		cout << "Enter your command: ";
		cin >> choose;
	} while (choose < 0 || choose >= menuItemCount);
	return choose;
}

void menuList() {
	ifstream is;
	is.open(file, ios::in | ios::binary);
	is.seekg(0);

	DataRow row(tableStudents);
	do {
		try {
			is >> row;
			cout << "Num:   " << *row.atColumn(colNum).getInt() << "\t";
			cout << "Name:  " << *row.atColumn(colName).getText() << "\t";
			cout << "Year:  " << *row.atColumn(colYear).getInt() << "\t";
			cout << "Grade: " << *row.atColumn(colGrade).getReal() << "\t";
			cout << endl;
		} catch (std::exception &_) {
			break;
		}
	} while (!is.eof());

	is.close();
}

void menuAdd() {
	static TypeInt num = 1;
	TypeText name;
	TypeInt year;
	TypeReal grade;
	cout << "Enter Name: ";
	cin >> name;
	cout << "Enter Year: ";
	cin >> year;
	cout << "Enter Grade: ";
	cin >> grade;

	DataRow row(tableStudents);
	row.atColumn(colNum).setValue(num++);
	row.atColumn(colName).setValue(name);
	row.atColumn(colYear).setValue(year);
	row.atColumn(colGrade).setValue(grade);

	ofstream os;
	os.open(file, ios::in | ios::app | ios::binary);
	os.seekp(0, os.end);
	os << row;
	os.close();
}

void menuFind() {
	cout << "Find" << endl;
}
