#
# Created: 29.09.11
#      by: Maksim Tamkovich
#

TEMPLATE = lib
TARGET = core
DESTDIR = ../../../lib/kirawnik/
CONFIG += warn_on
CONFIG += debug_and_release
QMAKE_CXXFLAGS += -std=c++0x
#QMAKE_CXXFLAGS_DEBUG += -O1

INCLUDEPATH = . ../../ ../

unix {
	TMP_BUILD_PATH = /tmp/$$TARGET
	MOC_DIR = $$TMP_BUILD_PATH
	RCC_DIR = $$TMP_BUILD_PATH
	OBJECTS_DIR = $$TMP_BUILD_PATH
}

HEADERS = \
		AbstractHeaderItem.h \
		Application.h \
		FilePanel.h \
		FileSystemView.h \
		FileView.h \
		HeaderView.h \
		MainWindow.h

SOURCES = \
		Application.cpp \
		FilePanel.cpp \
		FileSystemView.cpp \
		FileView.cpp \
		HeaderView.cpp \
		MainWindow.cpp
