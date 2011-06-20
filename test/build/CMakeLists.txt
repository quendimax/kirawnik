cmake_minimum_required (VERSION 2.6)

set (PROJECT test)

include_directories (../../src/)
include_directories (../)

set (HEADERS
	../../src/fs/FileInfo.h)

set (SOURCES
	../fs/FileInfo_test.cpp ../../src/fs/FileInfo.cpp)

add_executable (${PROJECT} ${HEADERS} ${SOURCES})

target_link_libraries (${PROJECT} gtest)
target_link_libraries (${PROJECT} gtest_main)
target_link_libraries (${PROJECT} pthread)
