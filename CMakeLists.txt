cmake_minimum_required(VERSION 3.10)
project(database)

set(CMAKE_CXX_STANDARD 14)

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
        Database.h
)

ADD_EXECUTABLE(database-test test/test-context.cpp)
target_link_libraries(database-test Database)

ADD_EXECUTABLE(database-bookstore test/test-bookstore.cpp)
target_link_libraries(database-bookstore Database)
