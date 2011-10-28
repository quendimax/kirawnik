#
# Created: 29.09.11
#      by: Maksim Tamkovich
#

include(../../../kirawnik.pri)

TEMPLATE = lib
CONFIG += shared
TARGET = core
VERSION = KIRAWNIK_VERSION
DESTDIR = ../../../lib/kirawnik/

INCLUDEPATH = . ../../ ../

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
