#ifndef DATABASE_DATA_VALUE_H
#define DATABASE_DATA_VALUE_H

#include "../MetaInfo/DataType.h"
#include "../MetaInfo/DataTypeIO.h"

namespace db {
	/// this DataValue can store any available data and data array
	/// it allocate required space at first and release it at the end
	struct DataValue {
		const bool typeChange = true;

		~ DataValue();

		DataValue(const DataValue &other);

		explicit DataValue(const DataType &type = TYPE_NONE, bool typeChange = true);

		bool operator<(const DataValue &rhs) const;

		bool operator>(const DataValue &rhs) const;

		bool operator==(const DataValue &rhs) const;

		friend std::ostream &operator<<(std::ostream &stream, const DataValue& val);

	private:
		DataType dataType = TYPE_NONE;
		void *value = nullptr;
		bool hasVal = false;

		DataValue &allocSpace();

		/// reset to not inited data and free storage
		DataValue &reset();

	public:
		/// clear value to 0 equivalent
		DataValue &clear();

		DataType type() const;

		DataValue &type(const DataType &newtype);


		bool isInited() const;

		bool isTypeVar() const;

		bool isDataVar() const;

		bool hasValue() const;

		TypeSize getCount() const;

		TypeSize getSize() const;

		const void *getValue() const;

		const TypeByte *getByte(size_t pos = 0) const;

		const TypeInt *getInt(size_t pos = 0) const;

		const TypeReal *getReal(size_t pos = 0) const;

		const TypeText *getText() const;


		DataValue &setValue(const void *source, const DataType &type, size_t count);

		DataValue &setValue(const void *val, size_t count);

		DataValue &setValue(const DataValue &from);


		DataValue &setValueByte(const TypeByte *val, size_t count = 1);

		DataValue &setValueByte(const TypeByte &val);


		DataValue &setValueInt(const TypeInt *val, size_t count = 1);

		DataValue &setValueInt(const TypeInt &val);


		DataValue &setValueReal(const TypeReal *val, size_t count = 1);

		DataValue &setValueReal(const TypeReal &val);


		DataValue &setValueText(const TypeText *val);

		DataValue &setValueText(const TypeText &val);

		int compare(const DataValue &other) const;

		bool isDataVariable() const;


		std::ostream &write(std::ostream &stream) {
			return writeData(stream, getValue(), dataType);
		}

		std::istream &read(std::istream &stream) {
			hasVal = !!readData(stream, value, dataType);
			return stream;
		}
	};
}
#endif