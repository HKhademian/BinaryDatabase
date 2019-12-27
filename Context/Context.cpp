#include<cstdarg>
#include<string>
#include <iostream>
#include <fstream>
#include "../utils.h"
#include "../MetaInfo/DatabaseInfo.h"
#include "Context.h"
#include "Result.h"
#include "Argument.h"

namespace db {
	namespace ctx {
		Context::Context(const Context *parent) : parent(parent) {}

		Context::~Context() {
			clear();

			// delete tablePos;
			table = nullptr;

			delete database;
			database = nullptr;
		}

		Context &Context::clear() {
			arguments.clear();

			columns.clear();

			dataRows.clear();

			result.type = Result::ERR;
			result.message.clear();

			// delete tablePos;
			// tablePos = nullptr;

			return self;
		}

		Context &Context::open(
			const std::string &name,
			int version,
			OnDatabaseOpen onOpen,
			OnDatabaseCreate onCreate,
			OnDatabaseMigrate onMigrate
		) {
			if (parent != nullptr) {
				throw std::logic_error("open database on parent context");
			}
			if (database != nullptr) {
				throw std::logic_error("close current database first");
			}

			database = new DatabaseInfo(name);
			try {
				const std::string path = "./db-" + name + "-inf.dat";
				std::ifstream is;
				is.open(path, std::ios::in | std::ios::binary);
				is >> *database;
				is.close();
			} catch (...) {
				delete database;
				database = new DatabaseInfo(name);
				saveDatabaseInfo();
				if (onCreate) {
					onCreate(self, version);
				}
			}

			if (onMigrate && database->version != version) {
				onMigrate(self, version, database->version);
			}

			if (onOpen) {
				onOpen(self, version);
			}

			return self;
		}

		Context &Context::close() {
			// TODO: some extra saving state process
			clear();

			if (parent == nullptr && database != nullptr) {
				delete database;
				database = nullptr;
				return self;
			}

			throw std::logic_error("no database is open in current context");
		}

		void Context::saveDatabaseInfo() const {
			const std::string path = "./db-" + database->name + "-inf.dat";
			std::ofstream os;
			os.open(path, std::ios::out | std::ios::trunc | std::ios::binary);
			os << *database;
			os.close();
		}

		Context &Context::exec(const std::string &cmd, ...) {
			clear();

			//TODO: implement varargs
			va_list vargs;
			va_start(vargs, cmd);

			parseCommandArgs(arguments, vargs, cmd);

			for (auto &arg : arguments) {
				std::cerr << "Arg#" << arg.index << std::endl;
				for (const auto &argRange: arg.ranges) {
					std::cerr << "    in [" << argRange.start << "," << argRange.end << "]=" << paramSub(cmd, argRange) << std::endl;
				}
			}

			va_end(vargs);

			return self;
			return snapEval(*this, cmd, Range(0, cmd.size() - 1));
		}


		const std::vector<Argument> &Context::args(bool include) const {
			return
				include && arguments.empty() && parent != nullptr ?
				parent->args(include) :
				arguments;
		}


		Context &Context::res(int type, const std::string &message) {
			result.type = type;
			result.message = message;
			return self;
		}

		Context &Context::done(const std::string &message) {
			return res(Result::DONE, message);
		}

		Context &Context::err(const std::string &message) {
			return res(Result::ERR, message);
		}

		Context &Context::todo(const std::string &message) {
			return err(message);
		}

		bool Context::hasError() {
			return result.type == Result::ERR;
		}

		const Result &Context::res(bool include) const {
			// include is not related YET
			return result;
		}


		const std::vector<ColumnInfo> &Context::cols(bool include) const {
			return
				include && columns.empty() && parent != nullptr ?
				parent->cols(include) :
				columns;
		}

		Context &Context::cols(std::vector<ColumnInfo> newcols) {
			self.columns = std::move(newcols);
			return self;
		}


		const std::vector<DataRow> &Context::rows(bool include) const {
			return
				include && dataRows.empty() && parent != nullptr ?
				parent->rows(include) :
				dataRows;
		}

		Context &Context::rows(std::vector<DataRow> newrows) {
			self.dataRows = std::move(newrows);
			return self;
		}


		const DatabaseInfo *Context::pdb(bool include) const {
			return
				database != nullptr ? database :
				include && parent != nullptr ? parent->pdb(include) :
				throw std::logic_error("database is not open");
		}

		const DatabaseInfo &Context::db(bool include) const {
			auto *pdatabase = pdb(include) ?: throw std::logic_error("database is not open");
			return *pdatabase;
		}


		const TableInfo *Context::ptbl(bool include) const {
			return
				table != nullptr ? table :
				include && parent != nullptr ? parent->ptbl(include) :
				throw std::logic_error("table is not selected");
		}

		const TableInfo &Context::tbl(bool include) const {
			auto *ptable = ptbl(include) ?: throw std::logic_error("table is not selected");
			return *ptable;
		}

		Context &Context::tbl(TableInfo *ptable) {
			self.table = table;
			return self;
		}
	}
}