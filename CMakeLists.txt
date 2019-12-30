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

file(GLOB Tests "test/*.h" "test/*.cpp")
ADD_EXECUTABLE(database-test ${Tests})
target_link_libraries(database-test Database)
