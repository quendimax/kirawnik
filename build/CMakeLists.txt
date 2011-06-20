cmake_minimum_required (VERSION 2.6)

set (PROJECT kirawnik)

set (HEADERS
	../src/fs/FileInfo.h)

set (SOURCES
	../src/main.cpp
	../src/fs/FileInfo.cpp)


project (${PROJECT})

include_directories (../src/)

find_package (Qt4 REQUIRED)
include (${QT_USE_FILE})
qt4_wrap_cpp (MOC_SOURCES ${HEADERS})

add_definitions (-Wall -std=c++0x -pedantic)

add_executable (${PROJECT} ${HEADERS} ${SOURCES} ${MOC_SOURCES})

target_link_libraries (${PROJECT} ${QT_LIBRARIES})