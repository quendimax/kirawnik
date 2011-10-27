#
# Created: 27.07.11
#      by: Maksim Tamkovich
#

TEMPLATE = app
TARGET = kirawnik
DESTDIR = ../../bin
CONFIG += warn_on
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
	TARGET = $${TARGET}d
	win32:CONFIG += console
}
QMAKE_CXXFLAGS += -std=c++0x
#QMAKE_CXXFLAGS_DEBUG += -O1

INCLUDEPATH = . .. ../libs/
LIBS += -L"../../lib/kirawnik/" -lcore -loptionsystem -lpluginsystem

unix {
	TMP_BUILD_PATH = /tmp/$$TARGET
	MOC_DIR = $$TMP_BUILD_PATH
	RCC_DIR = $$TMP_BUILD_PATH
	OBJECTS_DIR = $$TMP_BUILD_PATH
}

SOURCES = main.cpp
