#include<cstdarg>
#include<string>
#include <iostream>
#include "../utils.h"
#include "../MetaInfo/MetaInfo.h"
#include "Context.h"
#include "Result.h"
#include "Argument.h"

namespace db {
	namespace ctx {
		Context::Context(Context *parent)
			: parent(parent) {}

		Context::~Context() {
			clear();

			// delete table;
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

			// delete table;
			// table = nullptr;

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
				database->readInfo();
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

			// throw std::logic_error("no database is open in current context");
			return self;
		}

		void Context::saveDatabaseInfo() const {
			database->writeInfo();
		}

		Context &Context::exec(const char *cmd, ...) {
			va_list vargs;
			va_start(vargs, cmd);
			auto &res = execArgs(std::string(cmd), vargs);
			va_end(vargs);
			return res;
		}

		Context &Context::exec(const std::string &cmd, ...) {
			va_list vargs;
			va_start(vargs, cmd);
			auto &res = execArgs(cmd, vargs);
			va_end(vargs);
			return res;
		}

		Context &Context::execArgs(const std::string &cmd, va_list &vargs) {
			clear();
			parseCommandArgs(arguments, vargs, cmd);

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


		std::vector<ColumnInfo> &Context::getCols(bool include) {
			return
				!include || !columns.empty() ? columns :
				parent != nullptr ? parent->getCols(include) :
				columns;
		}

		Context &Context::setCols(std::vector<ColumnInfo> newcols) {
			self.columns = std::move(newcols);
			return self;
		}


		std::vector<DataRow> &Context::getRows(bool include) {
			return
				!include || !dataRows.empty() ? dataRows :
				parent != nullptr ? parent->getRows(include) :
				dataRows;
		}

		Context &Context::setRows(std::vector<DataRow> newrows) {
			self.dataRows = std::move(newrows);
			return self;
		}


		const DatabaseInfo *Context::getpDB(bool include) const {
			return
				!include || database != nullptr ? database :
				parent != nullptr ? parent->getpDB(include) :
				nullptr;
		}

		const DatabaseInfo &Context::getDB(bool include) const {
			auto *pdatabase = getpDB(include) ?: throw std::logic_error("database is not open");
			return *pdatabase;
		}


		TableInfo *Context::getpTable(bool include) {
			return
				!include || table != nullptr ? table :
				parent != nullptr ? parent->getpTable(include) :
				nullptr;
		}

		TableInfo &Context::getTable(bool include) {
			auto *ptable = getpTable(include) ?: throw std::logic_error("table is not selected");
			return *ptable;
		}

		Context &Context::setTable(const TableInfo *ptable) {
			self.table = const_cast<TableInfo *>(ptable);
			return self;
		}
	}
}