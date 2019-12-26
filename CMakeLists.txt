cmake_minimum_required(VERSION 3.10)
project(database)

set(CMAKE_CXX_STANDARD 14)

#add_library(
#        Database SHARED
#
#        MetaInfo/DataType.h MetaInfo/DataType.cpp
#        MetaInfo/DataTypeIO.h MetaInfo/DataTypeIO.cpp
#        MetaInfo/ColumnInfo.h MetaInfo/ColumnInfo.cpp
#        MetaInfo/TableInfo.cpp MetaInfo/TableInfo.h
#        MetaInfo/DatabaseInfo.cpp MetaInfo/DatabaseInfo.h
#        MetaInfo/DataCell.cpp MetaInfo/DataCell.h
#        MetaInfo/DataRow.cpp MetaInfo/DataRow.h
#
#        Context/utils.h Context/utils.cpp
#        Context/params.h Context/params.cpp
#        Context/Context.h Context/Context.cpp
#        Context/Result.h Context/Result.cpp
#        Context/Functions/CreateTable.cpp
#
#        utils.h utils.cpp
#        Database.cpp Database.h
#)

file(GLOB MetaInfo "MetaInfo/*.h" "MetaInfo/*.cpp")
file(GLOB Data "Data/*.h" "Data/*.cpp")
file(GLOB Context "Context/*.h" "Context/*.cpp")
file(GLOB ContextFunctions "Context/Functions/*.h" "Context/Functions/*.cpp")
add_library(
        Database SHARED

        ${MetaInfo}
        ${Data}
        ${Context}
        ${ContextFunctions}

        utils.h utils.cpp
        Database.cpp Database.h
)

ADD_EXECUTABLE(
        database-test
        test.cpp
        test-students.cpp
        # test-context.cpp
)
target_link_libraries(database-test Database)
