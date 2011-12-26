#
# Created: 29.09.11
#      by: Maksim Tamkovich
#

include(../../../kirawnik.pri)

TEMPLATE = lib
TARGET = $$getTarget(core)
CONFIG += shared
VERSION = $$KIRAWNIK_VERSION
DESTDIR = ../../../lib/kirawnik/

INCLUDEPATH = . ../../ ../

HEADERS = \
		AbstractHeaderItem.h \
		Application.h \
		FilePanel.h \
		FileSystemView.h \
		FileView.h \
		HeaderView.h \
		Hotkey.h \
		MainWindow.h

SOURCES = \
		AbstractHeaderItem.cpp \
		Application.cpp \
		FilePanel.cpp \
		FileSystemView.cpp \
		FileView.cpp \
		HeaderView.cpp \
		Hotkey.cpp \
		MainWindow.cpp
