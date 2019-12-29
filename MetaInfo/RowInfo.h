#ifndef DATABASE_ROW_INFO_H
#define DATABASE_ROW_INFO_H

#include <cstdlib>
#include "TableInfo.h"

namespace db {
	struct RowInfo {
		const TableInfo &table;

		explicit RowInfo(const TableInfo &table);

	protected:
		TypeSize offsetOnDisk = -1;
		TypeSize sizeOnDisk = -1;
	private:
		TypeSize minSize = -1;
		TypeFlag flag = 0;

	public:
		bool operator==(const RowInfo& rhs) const;
		bool operator<(const RowInfo& rhs) const;

		bool isFree() const;

		void setFree(bool isFree);

		size_t unique() const;

		std::istream &readInfo(std::istream &is);

		std::ostream &writeInfo(std::ostream &os);

		RowInfo &readInfo();

		RowInfo &writeInfo();
	};
}


#endif
