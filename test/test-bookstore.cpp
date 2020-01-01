#include<iostream>
#include <iomanip>
#include "../Database.h"

using namespace std;
using namespace db;
using namespace db::ctx;


int getChoose();

void showMenu();

void menuList(Context &);

void menuAdd(Context &);

void menuFind(Context &);

void menuRemove(Context &);

void menuUpdate(Context &);

void onDBOpen(Context &context, int version);

void onDBCreate(Context &context, int version);

void onDBMigrate(Context &context, int version, int oldVersion);

const auto DB_NAME = "bookstore";
const auto DB_VERSION = 1;

struct MenuItem {
	string title;

	void (*action)(Context &);
} const menuItems[] = {
	{"Exit",          nullptr},
	{"List    Books", menuList},
	{"Find    Books", menuFind},
	{"Add new Book",  menuAdd},
	{"Update  Book",  menuUpdate},
	{"Remove  Book",  menuRemove},
};

struct Book {
	static constexpr const auto TABLE = "books";
	static constexpr const auto COL_ID = "id", COL_ISBN = "isbn", COL_TITLE = "title", COL_WRITER = "writer";
	static constexpr const auto COL_YEAR = "year", COL_PRICE = "price", COL_FLAG = "flag";

	TypeInt id = 0, publishYear = 0;
	TypeText title, writer, isbn;
	TypeReal price = 0;
	TypeByte flag = 0;

	friend ostream &operator<<(ostream &stream, const Book &book) {
		cout << """Id: " << left << setfill(' ') << setw(4) << book.id << " ";
		cout << "| ISBN: " << left << setfill(' ') << setw(10) << book.isbn << " ";
		cout << "| Title: " << left << setfill(' ') << setw(10) << book.title << " ";
		cout << "| Writer: " << left << setfill(' ') << setw(10) << book.writer << " ";
		cout << "| Year: " << left << setfill(' ') << setw(4) << book.publishYear << " ";
		cout << "| Price: " << left << setfill(' ') << setw(4) << book.price << " ";
		cout << "| Taken: " << left << setfill(' ') << setw(3) << (book.flag == 1 ? "Yup" : "Nop");
		return stream;
	}

	friend Book &extract(const DataRow &row, Book &book) {
		book.id = *row.atColumn(COL_ID)->getInt();
		book.isbn = *row.atColumn(COL_ISBN)->getText();
		book.title = *row.atColumn(COL_TITLE)->getText();
		book.writer = *row.atColumn(COL_WRITER)->getText();
		book.publishYear = *row.atColumn(COL_YEAR)->getInt();
		book.price = *row.atColumn(COL_PRICE)->getReal();
		book.flag = *row.atColumn(COL_FLAG)->getByte();
		return book;
	}

};

int main() {
	Context context;
	context.open(DB_NAME, DB_VERSION, &onDBOpen, &onDBCreate, &onDBMigrate);
	while (true) {
		const auto choose = getChoose();
		const auto action = menuItems[choose].action;
		if (action == nullptr) {
			cout << "Good Bye!" << endl;
			break;
		}
		(*action)(context);
		system("pause");
	}
	context.close();
	return 0;
}

void onDBOpen(Context &context, int version) {

}

void onDBCreate(Context &context, int version) {
	string query;
	query += "CreateTable(";
	query += Book::TABLE;
	query += ", ";
	query += Book::COL_ID;
	query += ":int,";
	query += Book::COL_ISBN;
	query += ":text,";
	query += Book::COL_TITLE;
	query += ":text,";
	query += Book::COL_WRITER;
	query += ":text,";
	query += Book::COL_YEAR;
	query += ":int,";
	query += Book::COL_PRICE;
	query += ":real,";
	query += Book::COL_FLAG;
	query += ":byte";
	query += ")";
	auto &results = context.exec(query);
	if (results.hasError()) {
		cerr << "There is an error in database connection." << endl
		     << "please contact system admin for further fixes." << endl
		     << "err: " << results.res().message << endl;
		return;
	}
	results.clear();
}

void onDBMigrate(Context &context, int version, int oldVersion) {

}

void showMenu() {
	system("cls");
	int pos = 0;
	for (const auto &menuItem : menuItems) {
		cout << pos++ << ": " << menuItem.title << endl;
	}
}

int getChoose() {
	int choose = 0;
	do {
		showMenu();
		cout << "Enter your command: ";
		cin >> choose;
		cin.ignore(INT_MAX, '\n');
	} while (choose < 0 || choose >= sizeof(menuItems) / sizeof(MenuItem));
	return choose;
}

void printRows(const vector<DataRow> &rows) {
	if (rows.empty()) {
		cout << "Nothing found!" << endl;
		return;
	}
	cout << "(" << rows.size() << ") Books found: " << endl;
	int i = 1;
	Book book;
	for (auto &row: rows) {
		extract(row, book);
		cout << "#" << setfill('0') << setw(3) << right << i++ << ": " << book << endl;
	}
}

void menuList(Context &context) {
	string query;
	query += "select(";
	query += Book::TABLE;
	query += ")";
	auto &results = context.exec(query);
	if (results.hasError()) {
		cerr << "There is an error in database connection." << endl
		     << "please contact system admin for further fixes." << endl
		     << "err: " << results.res().message << endl;
		return;
	}
	auto &rows = results.getRows();
	printRows(rows);
	results.clear();
}

void menuAdd(Context &context) {
	Book book;
	book.flag = 0;

	cout << "Enter ID: ";
	cin >> book.id;
	cin.ignore(INT_MAX, '\n');

	cout << "Enter ISBN: ";
	getline(cin, book.isbn);

	cout << "Enter Title: ";
	getline(cin, book.title);

	cout << "Enter Writer: ";
	getline(cin, book.writer);

	cout << "Enter Publish Year: ";
	cin >> book.publishYear;
	cin.ignore(INT_MAX, '\n');

	cout << "Enter Price: ";
	cin >> book.price;
	cin.ignore(INT_MAX, '\n');

	string query;
	query += "Insert(";
	query += Book::TABLE;
	query += ", {";
	query += Book::COL_ID;
	query += "=${int},";
	query += Book::COL_ISBN;
	query += "=${text},";
	query += Book::COL_TITLE;
	query += "=${text},";
	query += Book::COL_WRITER;
	query += "=${text},";
	query += Book::COL_YEAR;
	query += "=${int},";
	query += Book::COL_PRICE;
	query += "=${real},";
	query += Book::COL_FLAG;
	query += "=${byte}";
	query += "})";

	auto &results = context.exec(query, book.id, book.isbn.c_str(), book.title.c_str(), book.writer.c_str(), book.publishYear, book.price, book.flag);
	if (results.hasError()) {
		cerr << "There is an error in database connection." << endl
		     << "please contact system admin for further fixes." << endl
		     << "err: " << results.res().message << endl;
		return;
	}
	cout << "Added!" << endl;
	results.clear();
}

void menuFind(Context &context) {
	void *target = nullptr;
	string what, typ;
	bool inp = true;
	while (inp) {
		cout << "FIELDS: id,isbn,title,writer,year,price,free" << endl;
		cout << "Please Enter what field you want to search or exit to return: " << endl;
		getline(cin, what);
		if (what == "id") {
			cout << "Enter ID to search: ";
			cin >> *(TypeInt *) (target = new TypeInt);
			inp = !cin;
			cin.ignore(INT_MAX, '\n');
			what = Book::COL_ID;
			typ = "int";
		} else if (what == "isbn") {
			cout << "Enter ISBN to search: ";
			getline(cin, *(TypeText *) (target = new TypeText));
			inp = !cin;
			what = Book::COL_ISBN;
			typ = "text";
		} else if (what == "title") {
			cout << "Enter Book Title to search: ";
			getline(cin, *(TypeText *) (target = new TypeText));
			inp = !cin;
			what = Book::COL_TITLE;
			typ = "text";
		} else if (what == "writer") {
			cout << "Enter value to search: ";
			getline(cin, *(TypeText *) (target = new TypeText));
			inp = !cin;
			what = Book::COL_WRITER;
			typ = "text";
		} else if (what == "price") {
			cout << "Enter Price to search: ";
			cin >> *(TypeReal *) (target = new TypeReal);
			cin.ignore(INT_MAX, '\n');
			inp = !cin;
			what = Book::COL_PRICE;
			typ = "real";
		}  else if (what == "year") {
			cout << "Enter Year to search: ";
			cin >> *(TypeInt *) (target = new TypeInt);
			cin.ignore(INT_MAX, '\n');
			inp = !cin;
			what = Book::COL_YEAR;
			typ = "real";
		} else if (what == "free") {
			cout << "Enter FreeState (0 or 1) to search: ";
			cin >> *(TypeByte *) (target = new TypeByte);
			inp = !cin;
			cin.ignore(INT_MAX, '\n');
			what = Book::COL_FLAG;
			typ = "byte";
		} else if (what == "exit") {
			return;
		}
	}

	string query;
	query += "Select(";
	query += Book::TABLE;
	query += ", eq(";
	query += what;
	query += ",${";
	query += typ;
	query += "}) )";

	Context *results = nullptr;
	if (typ == "byte") {
		results = &context.exec(query, *(TypeByte *) target);
		delete (TypeByte *) target;
	} else if (typ == "int") {
		results = &context.exec(query, *(TypeInt *) target);
		delete (TypeInt *) target;
	} else if (typ == "real") {
		results = &context.exec(query, *(TypeReal *) target);
		delete (TypeReal *) target;
	} else if (typ == "text") {
		results = &context.exec(query, ((TypeText *) target)->c_str());
		delete (TypeText *) target;
	}
	if (results == nullptr || results->hasError()) {
		cerr << "There is an error in database connection." << endl
		     << "please contact system admin for further fixes." << endl
		     << "err: " << (results ? results->res().message : "NULL") << endl;
		if (results)results->clear();
		return;
	}
	auto &rows = results->getRows();
	printRows(rows);
	results->clear();
}

void menuRemove(Context &context) {
	TypeInt id = 0;
	cout << "Enter ID to remove: ";
	cin >> id;
	cin.ignore(INT_MAX, '\n');
	string query;
	query += "Remove(";
	query += Book::TABLE;
	query += ", eq(";
	query += Book::COL_ID;
	query += ", ${int}))";
	auto &results = context.exec(query, id);
	if (results.hasError()) {
		cerr << "There is an error in database connection." << endl
		     << "please contact system admin for further fixes." << endl
		     << "err: " << results.res().message << endl;
		return;
	}
	auto &rows = results.getRows();
	if (!rows.empty())
		cout << "Removed (" << rows.size() << " Books)!" << endl;
	else
		cout << "Noting found to remove!" << endl;
	results.clear();
}

void menuUpdate(Context &context) {
	cerr << "Not Implemented yet!" << endl;
}
