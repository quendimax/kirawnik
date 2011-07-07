cmake_minimum_required (VERSION 2.6)

set (PROJECT test)
set (LIBS gtest gtest_main pthread)

project (${PROJECT})

include_directories (../../src/)

set (HEADERS
	../../src/fs/FileInfo.h
	../../src/fs/Directory.h)

set (SOURCES
	../fs/FileInfo_test.cpp ../../src/fs/FileInfo.cpp
	../fs/Directory_test.cpp ../../src/fs/Directory.cpp)

add_executable (${PROJECT} ${HEADERS} ${SOURCES})

target_link_libraries (${PROJECT} ${LIBS})
