cmake_minimum_required(VERSION 3.10)
project(database)

set(CMAKE_CXX_STANDARD 14)

add_library(
        Database SHARED
        DataType.h DataType.cpp
        ColumnInfo.h ColumnInfo.cpp
        TableInfo.cpp TableInfo.h
        DatabaseInfo.cpp DatabaseInfo.h
        DataCell.cpp DataCell.h
        DataRow.cpp DataRow.h
        #Context.cpp Context.h
        Database.cpp Database.h
        utils.h utils.cpp
        DataTypeIO.h DataTypeIO.cpp)

ADD_EXECUTABLE(
        database-test
        DataType.h DataType.cpp
        ColumnInfo.h ColumnInfo.cpp
        TableInfo.cpp TableInfo.h
        DatabaseInfo.cpp DatabaseInfo.h
        DataCell.cpp DataCell.h
        DataRow.cpp DataRow.h
        Database.cpp Database.h
        #Context.cpp Context.h
        utils.h utils.cpp
        test.cpp test-students.cpp
        DataTypeIO.h DataTypeIO.cpp)