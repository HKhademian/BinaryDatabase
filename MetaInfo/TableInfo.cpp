#include <iostream>
#include "DataTypeIO.h"
#include "ColumnInfo.h"
#include "TableInfo.h"
#include "../utils.h"

namespace db {
	std::ostream &operator<<(std::ostream &os, const TableInfo &data) {
		writeText(os, data.name);

		const auto count = data.columns.size();
		writeSize(os, count);

		loop (i, count) {
			os << data.columns[i];
		}

		return os;
	}

	std::istream &operator>>(std::istream &is, TableInfo &data) {
		data.columns.clear();

		readText(is, data.name);

		const size_t count = readSize(is);
		loop (i, count) {
			auto el = new ColumnInfo();
			is >> *el;
			data.columns.push_back(*el);
		}

		return is;
	}

	size_t TableInfo::getRowSize() const {
		const auto count = columns.size();
		size_t sum = 0;
		loop(i, count) {
			sum += columns[i].getRowSize();
		}
		return sum;
	}

	size_t TableInfo::getDataOffset(int index) const {
		size_t offset = 0;
		loop(i, index) {
			const auto &col = columns[i];
			offset += col.getRowSize();
		}
		return offset;
	}

	int TableInfo::column(const std::string &columnName) const{
		loop(i, columns.size()) {
			if (strcaseequal(columns[i].name, columnName)) {
				return i;
			}
		}
		return -1;
	}
}
