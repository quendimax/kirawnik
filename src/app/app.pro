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
#QMAKE_CXXFLAGS_DEBUG += -O1

INCLUDEPATH = . ..

unix:TMP_BUILD_PATH = /tmp/$$TARGET
win32:TMP_BUILD_PATH = c:/Windows/Temp/$$TARGET

MOC_DIR = $$TMP_BUILD_PATH
RCC_DIR = $$TMP_BUILD_PATH
OBJECTS_DIR = $$TMP_BUILD_PATH

#CONFIG += precompile_header
#PRECOMPILED_HEADER = pch.h

HEADERS = \
		Global.h \
		MainWindow.h \
		Application.h \
		FileSystemView.h \
		FilePanel.h \
		AbstractHeaderItem.h \
		HeaderView.h \
		FileView.h \
		PluginManager.h

SOURCES = \
		main.cpp \
		MainWindow.cpp \
		Application.cpp \
		FileSystemView.cpp \
		FilePanel.cpp \
		HeaderView.cpp \
		FileView.cpp \
		PluginManager.cpp
